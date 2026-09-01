FROM alpine:latest AS builder
RUN apk add --no-cache \
    g++ \
    cmake \
    make \
    spdlog-dev \
    gtest-dev
WORKDIR /app
COPY . .
RUN mkdir build && cd build && cmake .. && make -j$(nproc)

FROM alpine:latest
RUN apk add --no-cache spdlog
WORKDIR /app
COPY --from=builder /app/build/app .
EXPOSE 8080
CMD ["./app"]
