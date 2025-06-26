# python-style sum() 함수

## 개요

파이썬의 sum()함수는 iterable 타입을 받아 요소들의 합을 계산하여 리턴한다.

c++ 에는 std::accumulate() 함수가 비슷한 기능을 제공하는데, 직접 시작과 끝 iterator를 제공해야 해서 불필요하게 코드가 길어질 수 있다.

```
std::vector<int> data = {1, 2, 3, 4, 5}
auto value = std::accumulate(data.begin(), data.end(), 0);

// -> wanted:
auto value = sum(data);
```



## 상세 요구 사항

(1) python 의 sum 과 같이 직접 컨테이너를 지정하여 sum 계산
- `auto result = sum(vector<int>{1, 2, 3, 4, 5});`

(2) overflow-proof 대책
- sum 계산 중에 타입 T의 범위를 초과할 가능성이 있는 경우에 대한 대비

(3) iterator 타입은 아니지만 자주 사용되는 타입들도 지원
- 예: tuple 등..
