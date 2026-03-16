FROM fedora:latest AS builder
RUN dnf install -y gcc-c++ cmake make spdlog-devel gtest-devel
WORKDIR /app
COPY . .
RUN mkdir build && cd build && cmake .. && make -j$(nproc)

FROM fedora:latest
RUN microdnf install -y spdlog
WORKDIR /app
COPY --from=builder /app/build/app .
EXPOSE 8080
CMD ["./app"]
