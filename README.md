# 📁 물류관리 시스템 프로젝트

본 프로젝트는 편의점 체인의 유통 및 재고 관리 시스템을 위한 데이터베이스를 설계하고, 이를 실제 MySQL 환경에서 구현하는 것을 목표로 한다.

## 📁 Project 1. 물류관리 시스템의 논리적 설계

### 목표

편의점 체인의 상품, 매장, 고객, 재고, 주문 정보를 효율적으로 관리할 수 있는 데이터베이스 논리 구조를 설계한다.

### 구현사항

- ER Diagram 설계
- Relation Schema 작성

### 샘플 쿼리

1. **Product Availability** : 특정 상품을 보유한 매장 및 재고 수량 조회
2. **Top-Selling Items** : 지난 한 달간 각 매장에서 가장 많이 판매된 상품
3. **Store Performance** : 이번 분기 매출이 가장 높은 지점
4. **Vendor Statistics** : 가장 많은 제품을 공급한 업체 및 해당 상품 판매 수량
5. **Inventory Reorder Alerts** : 재고가 재주문 기준 이하인 상품 목록
6. **Customer Purchase Patterns** : 멤버십 고객이 커피와 함께 가장 자주 구매한 상품 Top 3
7. **Franchise vs. Corporate Comparison** : 프랜차이즈 중 가장 다양한 상품을 제공하는 매장과 본사 직영점 비교

## 📁 Project 2. ODBC를 활용한 물류관리 시스템 구현

### 목표

Project 1에서 설계한 구조를 기반으로 MySQL + ODBC 환경에서 실제 물류 관리 시스템을 구현한다.

### 구현사항

- Logical / Physical Schema 설계 및 BCNF 정규화
- MySQL Workbench를 활용한 데이터베이스 구축
- C/C++ 및 ODBC API를 이용한 SQL 질의 실행 프로그램 구현
