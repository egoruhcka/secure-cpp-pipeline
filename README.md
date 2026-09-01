# secure-cpp-pipeline (mem-server)

C++ приложение-сервер памяти с полным observability-стеком:

- **Prometheus** — сбор метрик приложения;
- **Loki + Promtail** — сбор логов;
- **Grafana** — визуализация (источники данных и дашборд разворачиваются автоматически как код);
- **werf** — сборка образа и деплой в Kubernetes одной командой.

Вся инфраструктура описана как код (Infrastructure as Code) и воспроизводится на чистой машине.

---

## Требования

| Инструмент | Зачем нужен |
|---|---|
| Docker | werf собирает образы через Docker daemon |
| [werf](https://werf.io) v2 | сборка и деплой |
| kubectl | работа с кластером |
| Helm v3 | установка стека мониторинга |
| git | werf читает файлы проекта из git |
| Локальный K8s-кластер | minikube / k3s / MicroK8s / kind |

---

## Быстрый старт

### 1. Подготовка окружения

```bash
sudo systemctl start docker        # запустить Docker
# запустить кластер (minikube start / k3s / microk8s и т.д.)
kubectl get nodes                  # убедиться, что нода Ready
```

### 2. Установить мониторинг (Prometheus + Grafana)

```bash
helm repo add prometheus-community https://prometheus-community.github.io/helm-charts
helm repo update

helm install monitoring prometheus-community/kube-prometheus-stack \
  --namespace monitoring --create-namespace
```

> ⚠️ Этот шаг **обязателен до деплоя приложения**: чарт ставит CRD `ServiceMonitor`, без которого Helm-чарт приложения не установится (`no matches for kind "ServiceMonitor"`).

### 3. Установить сбор логов (Loki + Promtail)

```bash
helm repo add grafana https://grafana.github.io/helm-charts
helm repo update

helm install loki grafana/loki-stack \
  --namespace monitoring \
  --set promtail.enabled=true \
  --set loki.auth_enabled=false
```

> ⚠️ **Важно:** `loki-stack` создаёт ConfigMap, который помечает Loki как datasource «по умолчанию» и конфликтует с Prometheus — Grafana падает в CrashLoopBackOff с ошибкой
> `Only one datasource per organization can be marked as default`.
> Удаляем конфликтующий ConfigMap:

```bash
kubectl delete configmap loki-loki-stack -n monitoring
```

### 4. Задеплоить приложение

```bash
# если репозиторий образов приватный — сначала:
# werf login ghcr.io

werf converge --dev --repo=ghcr.io/egoruhcka/secure-cpp-pipeline
```

Что развернётся:

| Ресурс | Назначение |
|---|---|
| `Deployment` + `Service` (namespace `mem-server`) | 3 реплики приложения, порт 8080 |
| `ServiceMonitor` | Prometheus начинает скрейпить `/metrics` |
| `ConfigMap/grafana-loki-datasource` | Loki автоматически подключается в Grafana |
| `ConfigMap/mem-server-dashboard` | Дашборд автоматически появляется в Grafana |

### 5. Открыть Grafana

```bash
kubectl port-forward svc/monitoring-grafana -n monitoring 3000:80
```

- Адрес: **http://localhost:3000**
- Логин: `admin`, пароль: `prom-operator`

Дашборд **memory-server** появится в *Dashboards → Browse* автоматически (provisioned, только чтение — правки вносятся через git).

---

## Структура проекта

```
├── werf.yaml                      # конфигурация сборки/деплоя werf
├── werf-giterminism.yaml          # разрешение не закоммиченных файлов (dev)
├── Dockerfile                     # multi-stage сборка C++
├── src/                           # исходники C++ приложения
└── .helm/
    ├── Chart.yaml
    └── templates/
        ├── deployment.yaml        # 3 реплики mem-server
        ├── service.yaml           # Service (порт 8080)
        ├── servicemonitor.yaml    # скрейпинг метрик Prometheus
        ├── grafana-loki-datasource.yaml   # авто-подключение Loki в Grafana
        └── grafana-dashboard.yaml # авто-дашборд (метрики + логи)
```

---

## Метрики и логи приложения

Метрики в формате Prometheus отдаются на `GET /metrics` (порт 8080):

- `node_memory_MemTotal_bytes`
- `node_memory_MemFree_bytes`
- `node_memory_MemAvailable_bytes`

Логи (spdlog) пишутся в **stdout** — Promtail собирает их и отправляет в Loki.

---

## Troubleshooting

| Симптом | Решение |
|---|---|
| `no matches for kind "ServiceMonitor"` | Сначала установить kube-prometheus-stack (шаг 2) |
| Grafana в CrashLoopBackOff, «Only one datasource … default» | `kubectl delete cm loki-loki-stack -n monitoring` |
| werf: `werf.yaml not found` | Запускать команды из корня проекта; проверить `werf-giterminism.yaml` |
| werf: `Cannot connect to the Docker daemon` | `sudo systemctl start docker` |
| Дашборд не появляется в Grafana | Логи sidecar: `kubectl logs deploy/monitoring-grafana -n monitoring -c grafana-sc-dashboard --tail=50`; проверить валидность JSON дашборда: `python3 -m json.tool` |
| `loki-0` долго `0/1` | Подождать 1–2 минуты — Loki стартует медленно |

### Полезные команды

```bash
kubectl get pods -n monitoring     # состояние стека мониторинга
kubectl get pods -n mem-server     # состояние приложения

# UI Prometheus (метрики, таргеты скрейпинга)
kubectl port-forward svc/monitoring-kube-prometheus-prometheus -n monitoring 9090:9090
```

---
Сам сервер написан максимально модульно, так что добавить новые эндпоинты не составит труда. Запросы обрабатываются асинхронно, через пул потоков.