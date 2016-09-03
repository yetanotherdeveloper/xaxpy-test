# xaxpy-test

Simple test to compare efficency of BLAS IAXPY and SAXPY written in C++ vs Assembly implementations

Assembly routines are using Just In Time Assembly (Assembly generated in runtime) project [Xbyak](https://github.com/yetanotherdeveloper/xaxpy-test/)

## Installation notes
As xaxpy-test rely on xbyak project, this repo has xbyak added as git submodule. To have xbyak checked out next to xaxpy-test please please use following command:
*git clone --recursive https://github.com/yetanotherdeveloper/xaxpy-test.git*

### building xaxpy-test
1. mkdir <mybuild>; cd <mybuild>
2. cmake <Path to CMakeLists.txt in xaxpy-test directory> -DCMAKE_BUILD_TYPE=Release
3. make

# License
Copyright (c) 2016, Jacek Czaja
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

