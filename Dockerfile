FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC

# Install dependencies
RUN apt-get update && apt-get install -y \
    software-properties-common \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    qttools5-dev \
    qttools5-dev-tools \
    libqt5network5 \
    qtmultimedia5-dev \
    x11-apps \
    xvfb \
    libxcb1 libx11-xcb1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY /SkullKing /app

# Setup Qt project
RUN qmake -project
RUN sed -i '8 i QT += core gui widgets network' SkullKing.pro
RUN qmake SkullKing.pro
RUN make

# Run Xvfb in the background
CMD Xvfb :1 -screen 0 1024x768x24 & sleep 5 && ./SkullKing

