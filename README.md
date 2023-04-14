# 3D pong
Multiplayer 3D pong game using Vulkan (base code can be found here: https://github.com/DiantArts/VulkanSandbox)

# How to run
Run the following commands to install the dependencies (ubuntu-22.04 example):
```
apt-get install -y python3-pip cppcheck clang-tidy ccache moreutils libxvmc-dev cmake gcovr llvm
```
```
pip install conan ninja
```

Launch the server first (compiling the client first prevent the compilation of the server - sorry):
```
./.toolchain/compile --server
```
Launch the client
```
./.toolchain/compile
```

Use `-d` to enable debug.
Use `-h` for help.
