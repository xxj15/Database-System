#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    const char *server = "localhost";
    const char *user = "root";
    const char *password = "1234";  
    const char *database = "store";   

    // MySQL 초기화
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return 1;
    }

    // MySQL 서버 연결
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    int select_query;
    char input[100];
    char query[4096];

    while (1) {
        // 쿼리 선택
        printf("\n------------ SELECT QUERY TYPES ------------\n");
        printf("1. TYPE 1\n");
        printf("2. TYPE 2\n");
        printf("3. TYPE 3\n");
        printf("4. TYPE 4\n");
        printf("5. TYPE 5\n");
        printf("6. TYPE 6\n");
        printf("7. TYPE 7\n");
        printf("0. QUIT\n\n");
        printf("Select: ");

        scanf("%d", &select_query); getchar(); 

        // 프로그램 종료
        if (select_query == 0) {
            printf("Program terminated.\n");
            mysql_close(conn);
            return 0;
        }

        switch (select_query) {
            // 쿼리 1. Product Availability - 특정 상품을 보유하고 있는 매장 & 매장에서 보유하고 있는 재고 출력
            case 1:
                printf("------- TYPE 1 -------\n");
                printf("** Which stores currently carry a certain product (by UPC), and how much inventory do they have ? **\n");
                printf("커피류 : 10000001~10000004 / 디저트류 : 20000001~20000006 / 식품류 : 30000001~30000010\n");
                printf("Enter product identifier (UPC only): ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0';

                snprintf(query, sizeof(query),
                    "SELECT Stores.Name AS StoreName, Inventory.Quantity " 
                    "FROM Inventory "
                    "JOIN Stores ON Inventory.Store_ID = Stores.ID "
                    "WHERE Inventory.UPC = '%s';", input);
                break;

            // 쿼리2. Top-Selling Items - 지난 한 달동안 각 매장에서 가장 많이 판매된 상품 (2025년.6월.13일 기준)
            // 2025년 5월 13일 이후로 판매 이력 없는 경우 표시되지 않음. 
            case 2:
                printf("------- TYPE 2 -------\n");
                printf("** Which products have the highest sales volume in each store over the path month? (since 2025-05-13) **\n");
               strcpy(query,
                    "SELECT StoreName, Product, Quantity " // 가게 이름, 물건 이름, 물건이 판매된 수량 출력 
                    "FROM ("
                        "SELECT Stores.Name AS StoreName, Products.Name AS Product, SUM(SalesItem.Quantity) AS Quantity, " 
                            "RANK() OVER (PARTITION BY Stores.ID ORDER BY SUM(SalesItem.Quantity) DESC) AS ranking " // 매장별로 판매량 기준으로 내림차순 정렬
                        "FROM SalesItem " 
                        "JOIN Products ON  Products.UPC = SalesItem.UPC " // 상품 정보 연결 (UPC 기준)
                        "JOIN SalesTransaction ON SalesItem.Transaction_ID = SalesTransaction.ID " // 거래 내역 테이블 조인
                        "JOIN Stores ON SalesTransaction.Store_ID = Stores.ID " // 매장 정보 테이블 조인
                        "WHERE SalesTransaction.SalesDate >= '2025-05-13' " // 기준: 2025-05-13
                        "GROUP BY Stores.ID, Products.Name" // 매장별 -> 상품별 그룹화
                    ") AS TopSelling " 
                    "WHERE ranking = 1;" // 하나만 출력
                );
                break;

            // 쿼리3. Store Performance - 이번 분기 가장 매출이 높았던 지점 (2025년 4월 ~ 6월)
            case 3:
                printf("------- TYPE 3 -------\n");
                printf("** Which store has generated the highest overall revenue this quarter? **\n");
                strcpy(query,
                    "SELECT Stores.Name, SUM(SalesItem.Quantity * Products.Price ) AS TotalRevenue " // 매장 이름, 총 매출
                    "FROM SalesItem " 
                    "JOIN Products ON SalesItem.UPC = Products.UPC " // 상품의 UPC 기준으로 SalesItem 연결
                    "JOIN SalesTransaction ON SalesItem.Transaction_ID = SalesTransaction.ID " // SalesTransaction ID 기준으로 거래 물품 연결
                    "JOIN Stores ON Stores.ID = SalesTransaction.Store_ID  " // Store의 ID 기준으로 매장과 거래 내역 연정보 결
                    "WHERE SalesTransaction.SalesDate BETWEEN '2025-04-01' AND '2025-06-30' " // 이번 분기 : 2025년 4월 ~ 6월
                    "GROUP BY Stores.Name " // 매장 이름으로 grouping
                    "ORDER BY TotalRevenue DESC LIMIT 1;" // 매출 가장 높았던 지점 선택 
                );
                break;

            // 쿼리4. Vendor Statistics - 가장 많은 제품을 공급한 공급업체와 판매 수량
            case 4:
                printf("------- TYPE 4 -------\n");
                printf("** Which Vendor supplies the most products across the chain, and how many total units have been sold? **\n");
                strcpy(query,
                    "SELECT Vendor.Name, COUNT(DISTINCT Products.UPC) AS ProductTypes, SUM(SalesItem.Quantity) AS SoldQuantity " // UPC 기준으로 물건 종류 구하기 
                    "FROM Vendor "
                    "JOIN Products ON Vendor.ID = Products.Vendor_ID " // Vendor의 ID 기준 조합
                    "LEFT JOIN SalesItem ON Products.UPC = SalesItem.UPC "
                    "GROUP BY Vendor.ID, Vendor.Name " // Vendor의 ID와 Name 기준 Grouping
                    "ORDER BY SoldQuantity DESC LIMIT 1;"); // 상위 1개 공급업체 
                break;

            // 쿼리 5. Inventory Reorder Alerts - 재고가 재주문 기준 이하여서 물량 보충 필요한 것
            case 5:
                printf("------- TYPE 5 -------\n");
                printf("** Which products in each store are below the reorder threshold and need resocking?**\n");
                strcpy(query,
                    "SELECT Stores.Name AS StoreName, Products.Name AS ProductName, (Inventory.MinimumStock - Inventory.Quantity) AS NeedAmount " // 매장명, 제품명, 보충해야하는 개수 
                    "FROM Inventory " // 재고
                    "JOIN Stores ON Inventory.Store_ID = Stores.ID " // 매장
                    "JOIN Products ON Inventory.UPC = Products.UPC " 
                    "WHERE Inventory.Quantity < Inventory.MinimumStock " // 현재 재고 수량이 최소 필요 재고의 수보다 적은 경우 
                    "ORDER BY Stores.Name, Products.Name;"
                );
                break;

            // 쿼리6. Customer Purchase Patterns - 멤버십 고객이 커피와 함께 구매한 상위 3가지 물건 
            case 6: {
                printf("------- TYPE 6 -------\n");
                printf("** List the Top 3 items that loyalty program customers typically purchase with coffee. **\n");
                printf("1. Wonka Americano\n2. Wonka Latte\n3. Charlie Americano\n4. Charlie Latte\nEnter choice: ");
                int choice;
                scanf("%d", &choice); getchar();

                const char *coffeetype = NULL;
                // Products에 커피의 종류는 4가지만 포함하였다. 
                // 사용자가 선택하기 편하기 위해 1,2,3,4 로 입력할 수 있도록 하였다.
                switch (choice) {
                    case 1: coffeetype = "Wonka Americano"; break;
                    case 2: coffeetype = "Wonka Latte"; break;
                    case 3: coffeetype = "Charlie Americano"; break;
                    case 4: coffeetype = "Charlie Latte"; break;
                    default:
                        printf("Please Choose Another Product.\n");
                        continue;
                }

                snprintf(query, sizeof(query),
                    "SELECT Product " 
                    "FROM ("
                        "SELECT Products2.Name AS Product, COUNT(*) AS Quantity "
                        "FROM SalesTransaction ST "
                        "JOIN Customer C ON ST.Customer_ID = C.ID " // 고객 정보 연결 
                        "JOIN SalesItem item1 ON ST.ID = item1.Transaction_ID " // 첫 번째 구매 항목  - 거래 항목에 커피 들어있는지 확인
                        "JOIN Products Products1 ON item1.UPC = Products1.UPC "
                        "JOIN SalesItem item2 ON ST.ID = item2.Transaction_ID " // 두 번쨰 구매 항목 - 커피와 함께 구매한 항목들 확인
                        "JOIN Products Products2 ON item2.UPC = Products2.UPC "
                        "WHERE C.Membership = 1 AND Products1.Name = '%s' AND Products2.Name != Products1.Name " // 멤버십 여부 확인 & 커피 구매 확인 & 커피와 함께 구매한 항목 확인
                        "GROUP BY Product "
                        "ORDER BY Quantity DESC LIMIT 3" // 3개로 개수 제한
                    ") AS MostSellingProduct;", coffeetype);
                break;
            }

            // 쿼리7 - Franchise vs. corporate comparison - 프랜차이즈 매장 중 가장 다양한 상품을 제공하는 매장을 본사와 비교 
            case 7:
                printf("------- TYPE 7 -------\n");
                printf("** Among franchise-owned stores, which one offers the widest variety of products, and how that compare to corporate-owned stores? **\n");
                strcpy(query,
                    "SELECT isCorporate, StoreName, InventoryVarietyNumbers "
                    "FROM ("
                        "SELECT isCorporate, Name as StoreName, COUNT(DISTINCT Inventory.UPC) AS InventoryVarietyNumbers, " // 프랜차이즈 여부, 매장 이름, 재고의 종류
                        // RANK() OVER - PARTITION BY 사용
                        "RANK() OVER (PARTITION BY isCorporate " // 프랜차이즈와 직영점 구분
                        "ORDER BY COUNT(DISTINCT Inventory.UPC) DESC) AS ranking " //  재고 종류의 개수에 따라 내림차순 (기준: UPC)
                        "FROM Stores "
                        "JOIN Inventory ON Stores.ID = Inventory.Store_ID " // 매장의 재고 파악 
                        "GROUP BY Stores.ID"
                    ") InventoryCount "
                    "WHERE ranking = 1;"
                );
                break;

            default:
                printf("Enter Again Please\n");
                continue;
        }

        // 쿼리 실행
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Query failed. Error: %s\n", mysql_error(conn));
            continue;
        }

        // 결과 저장
        res = mysql_store_result(conn);
        if (res == NULL) {
            fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
            continue;
        }

        // 필드 출력
        int num_fields = mysql_num_fields(res);
        MYSQL_FIELD *fields = mysql_fetch_fields(res);
        for (int i = 0; i < num_fields; i++) {
            printf("%s\t", fields[i].name);
        }
        printf("\n");

        // 결과 행 출력
        while ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {
                printf("%s\t", row[i] ? row[i] : "NULL");
            }
            printf("\n");
        }

        // 리소스 해제
        mysql_free_result(res);
    }

    mysql_close(conn);
    return 0;
}