FROM fedora:25

WORKDIR /home/build
#Setup
RUN set -euxo pipefail; dnf -y update && dnf -y install \
git \
qt5-qtbase \
qt5-qtserialport-devel \
qt5-qtserialport \ 
qt5-qtcharts-devel \
qt5-qtcharts \
cmake extra-cmake-modules && \
git clone https://github.com/KDE/atcore.git && \
cmake atcore && make install
CMD ./testclient/AtCoreTest
