
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
- 각 방식을 어떻게 사용는지에 대해 간단한 설명 문서 필요. TODO

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

