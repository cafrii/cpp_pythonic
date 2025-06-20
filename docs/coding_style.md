
## 폴더 구조
- docs/
- src/
- TODO

## 이름 규칙
- 파일 이름
- 클래스 이름
- 함수 이름
- TODO

## namespace
- 별도의 특별한 이유가 없는 한 모든 코드는 동일한 하나의 네임스페이스로 감싸서 작성한다.
- 이 라이브러리의 namespace: "com::cafrii::pyc"
  - pyc 의미: 리포지토리의 이름인 "cpp_pythonic"를 적절히 축약하여 조합한 단어.

## 헤더/소스 파일
- header-only 또는 또는 선언/구현 분리 를 필요에 따라 선택할 수 있도록 한다.
- 헤더 온리 모드
  - 사용하는 측에서, 별다른 선언 없이 그냥 pycpp의 필요한 헤더 파일을 include 하여 사용한다.
- 선언/구현 분리 모드
  - 사용하는 측에서, __PYC_LIB_IMPLEMENTATION 를 먼저 정의한 후 헤더 파일을 포함한다.
  - 그 후 최종적으로는 pycpp 빌드 결과로 생성되는 라이브러리 파일을 링크 시켜 사용해야 한다.

## 의존성
- 가급적 pycpp 만으로 동작할 수 있도록 노력한다.
- 불가피한 외부 라이브러리 의존성이 필요한 경우, cmake 옵션으로 일부 기능을 비활성화 하여 의존성을 없애는 옵션도 제공한다.

## 부분 사용
- 특정 상황에서 header-only 로 아주 일부 기능만 사용하고자 하는 경우도 고려할 수 있다.
- 예: 다른 모든 기능은 다 필요 없고 BigInt 클래스만 사용하고자 하는 경우, 헤더 파일 하나만 포함시켜서 쓸 수 있도록 한다.


## 코딩 스타일

- 들여쓰기: 공백 사용, 4 글자 단위로 들여쓰기.
- 라인길이: 80자 이내 권장. 그러나 강제하지는 않음.

### 참고
- 구글 C++: https://google.github.io/styleguide/cppguide.html
- 모질라: https://firefox-source-docs.mozilla.org/code-quality/coding-style/index.html#

