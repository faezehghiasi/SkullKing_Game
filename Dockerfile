FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC

RUN apt-get update && apt-get install -y software-properties-common && \
    add-apt-repository universe && \
    apt-get update && apt-get install -y \
    build-essential qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools \
    qttools5-dev qttools5-dev-tools \
    libqt5network5 qtmultimedia5-dev \
    x11-apps xvfb \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN qmake -project
RUN sed -i '8 i QT += core gui widgets network' SkullKing.pro
RUN qmake SkullKing.pro
RUN make

ENV DISPLAY=:1
RUN Xvfb :1 -screen 0 1024x768x24 &

CMD ["./SkullKing"]
