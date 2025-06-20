


## 단위 시험 요구 사항
- 각 모듈 별 시험 파일이 제공되어야 한다.
  - 예: xyz 모듈을 작성한 경우, test_xyz.cpp 를 작성.
- 시험 대상 모듈이 제공하는 기능을 최대한 빠짐 없이 시험할 수 있어야 한다.
-


## 빌드

```
cd <repo_top>
mkdir out # any name can be used
cd out
cmake ..  # should specify top-level cmakelists.txt
make -j8

```


## 단위 시험
- tests/test_big_integer





