# NuttX 다운로드

```bash
mkdir nuttx
$ cd nuttx
$ git clone https://github.com/apache/incubator-nuttx.git nuttx
$ git clone https://github.com/apache/incubator-nuttx-apps apps
$ git clone https://bitbucket.org/nuttx/tools.git tools
```

# kconfig-frontends 패키지 설치
NuttX는 kconfig 를 사용해서 menuconfig.

```bash
$ apt install kconfig-frontends
```

# 사용 가능한 NuttX Base Configuration 목록 보기

```bash
$ cd nuttx
$ ./tools/configure.sh -L | less
```

#  configuration 초기화

```bash
$ cd nuttx
$ ./tools/configure.sh -l <board-name>:<config-dir>
# 예제:
$ ./tools/configure.sh -l sama5d27-xult:nsh
```

여기서 -l 옵션은 linux에서 빌드한다는 뜻이다.

# (선택) configuration을 커스터마이징

```bash
$ make menuconfig
```

# NuttX 컴파일

```bash
$ make
```

# binary 보드에 넣기


