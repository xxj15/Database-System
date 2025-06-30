## [Project 2] Implement Database for Convenience Store system Chain

### CSE4110 - Database Systems Spring 2025 | 20200271 안서진

### 💁🏻‍♀️ 편의점 유통 시스템을 만들기

### 📍 응답할 수 있는 쿼리

1. **Product Availability** : 특정 상품을 보유하고 있는 매장 & 매장에서 보유하고 있는 재고 출력

- 사용자가 UPC를 직접 입력해야 함
  - ☕️ 커피류 : 10000001~10000004
  - 🥖 디저트류 : 20000001~20000006
  - 🍙 식품류 : 30000001~30000010

2. **Top-Selling Items** : 지난 한 달동안 각 매장에서 가장 많이 판매된 상품 (2025년.6월.13일 기준)

- 2025년 5월 13일 이후로 판매 이력 없는 매장의 경우 표시되지 않음

3. **Store Performance** : 이번 분기 가장 매출이 높았던 지점 (2025년 4월 ~ 6월)

4. **Vendor Statistics** : 가장 많은 제품을 공급한 공급업체와 판매 수량

5. **Inventory Reorder Alerts** : 재고가 재주문 기준 이하여서 물량 보충 필요한 것

6. **Customer Purchase Patterns** : 멤버십 고객이 커피와 함께 구매한 상위 3가지 물건

- 사용자가 아래 중 커피 종류를 선택 입력해야 함 ☕️
  - Wonka Americano
  - Wonka Latte
  - Charlie Americano
  - Charlie Latte

7. **Franchise vs. Corporate Comparison** : 프랜차이즈 매장 중 가장 다양한 상품을 제공하는 매장을 본사와 비교

### 📍 실행 환경

- macOS Sequoia 15.1 (Apple Silicon)
- Apple M4 Pro Chip, Memory 24GB
- MySQL Ver 8.0.42 (arm64)
- Homebrew를 통해 `mysql-client` 설치

### 📍컴파일 방법

아래 명령어를 터미널에서 실행하여 컴파일할 수 있습니다.
MySQL C API 연동을 위해 mysql-client의 include 및 lib 경로를 명시했습니다.

### 📍 실행 명령어

```
gcc main.c -o app -I/opt/homebrew/opt/mysql-client/include -L/opt/homebrew/opt/mysql-client/lib -lmysqlclient
```

- `main.c` 를 컴파일하여 실행 파일 app을 생성하며, MySQL C API 사용을 위해 mysql-client의 헤더와 라이브러리 경로를 지정

`./app`

- 컴파일된 실행 파일 `app` 을 실행하여 프로그램을 시작

### 추가 사항

- `#include <mysql/mysql.h>` 헤더 경로 인식을 위해 `c_cpp_properties.json` 파일에 다음과 같이 추가합니다.

```
{
  "configurations": [
    {
      "name": "Mac",
      "includePath": [
        "${workspaceFolder}/**",
        "/opt/homebrew/opt/mysql-client/include"
      ],
      "defines": [],
      "compilerPath": "/usr/bin/clang",
      "cStandard": "c11",
      "cppStandard": "c++17",
      "intelliSenseMode": "macos-clang-arm64"
    }
  ],
  "version": 4
}
```

- `c_cpp_properties.json` 은 컴파일에는 필요하지 않지만, VS Code에서 `#include <mysql/mysql.h>` 헤더 경로 인식을 위해 실행 과정에서는 사용하였습니다.
