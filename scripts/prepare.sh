sudo apt update
sudo apt install -y binutils python3 python3-pip git help2man \
                    perl python3 make libfl2 libfl-dev zlibc zlib1g zlib1g-dev \
                    ccache mold libgoogle-perftools-dev numactl \
                    perl-doc autoconf flex bison clang clang-format \
                    cmake gdb graphviz lconv gtkwave unzip zip

PROJECT_PATH=$(pwd)
mkdir -p ~/sofware && cd ~/software && git clone https://github.com/verilator/verilator && cd verilator
autoconf && ./configure && make -j
echo "export VERILATOR_ROOT=${HOME}/softwares/verilator" >> ~/.zshrc
echo "export PATH=${VERILATOR_ROOT}/bin:$PATH" >> ~/.zshrc
cd ${PROJECT_PATH}

curl -s "https://get.sdkman.io" | bash
sdk version
sdk update && sdk install java 21.0.1-tem