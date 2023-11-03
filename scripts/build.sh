if [ "$#" -gt 1 ]; then
    echo "Usage: install"
    exit 1
elif [ "$#" -eq 1 ]; then
    key="$1"
    if [ "$key" != "install" ]; then
        echo "Usage: install"
        exit 1
    fi
    mkdir -p build && cd build && cmake .. && make -j  && sudo make install && cd ..
else
    mkdir -p build && cd build && cmake .. && make -j  && cd ..
fi
