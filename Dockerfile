FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=America/New_York
RUN apt-get update && apt-get install -y \
  tzdata \
  build-essential \ 
  cmake \
  libncurses5-dev 

