
FROM gcc:13

RUN apt-get update && apt-get install -y --no-install-recommends \
    libcurl4-openssl-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY src/ ./src/
COPY include/ ./include/
COPY data/ ./data/

RUN g++ src/*.cpp -Iinclude -std=c++17 -lcurl -o search_engine

ENV PORT=10000

EXPOSE 10000

CMD ["./search_engine", "--api"]
