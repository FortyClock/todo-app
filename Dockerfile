
FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    make \
    cmake \
    pkg-config \
    libpqxx-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY CMakeLists.txt .
COPY main.cpp .

RUN cmake -S . -B build && cmake --build build

CMD ["./build/todo"]
