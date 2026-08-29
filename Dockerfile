FROM fedora:latest AS builder
RUN dnf install -y gcc-c++ cmake make spdlog-devel gtest-devel ccache && \
    dnf clean all
WORKDIR /app
COPY . .
RUN mkdir build && cd build && cmake .. && make -j$(nproc)

FROM fedora-minimal:latest
RUN microdnf install -y spdlog && microdnf clean all
WORKDIR /app
COPY --from=builder /app/build/app .
EXPOSE 8080
CMD ["./app"]
