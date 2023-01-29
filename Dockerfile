FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
  build-essential autoconf git pkg-config \
  automake libtool curl make g++ unzip \
  && apt-get clean
RUN apt install -y cmake
# install protobuf first, then grpc
RUN git clone --recurse-submodules -b v1.50.0 \
    --depth 1 --shallow-submodules https://github.com/grpc/grpc && \
    cd grpc && mkdir -p cmake/build && cd cmake/build && \
    cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../.. && \
    make -j$(nproc) && make install && \
    echo "--- installed grpc ---"
RUN apt-get install -y libcurl4-openssl-dev
WORKDIR /Optimizer
COPY . .
RUN mkdir -p ./build && cd ./build && cmake .. && make -j$(nproc)
EXPOSE 50051
CMD ["./bin/server"]