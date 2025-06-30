DROP TABLE IF EXISTS SalesItem;
DROP TABLE IF EXISTS SalesTransaction;
DROP TABLE IF EXISTS Inventory;
DROP TABLE IF EXISTS Products;
DROP TABLE IF EXISTS Vendor;
DROP TABLE IF EXISTS Customer;
DROP TABLE IF EXISTS Stores;

-- 각 편의점 매장의 기본 정보를 저장
CREATE TABLE Stores (
    ID INT PRIMARY KEY,
    Name VARCHAR(50) NOT NULL,
    Address VARCHAR(100) NOT NULL,
    OperatingHours VARCHAR(50) NOT NULL,
    isCorporate BOOLEAN NOT NULL
);

-- 고객 관련 정보 저장 
CREATE TABLE Customer (
    ID INT PRIMARY KEY,
    Name VARCHAR(50) NOT NULL,
    Membership BOOLEAN NOT NULL
);

-- 제품을 납품하는 공급 업체 
CREATE TABLE Vendor (
    ID INT PRIMARY KEY,
    Name VARCHAR(50) NOT NULL,
    Phone VARCHAR(20) NOT NULL
);

-- 판매되는 각 상품 정보 저장 (브랜드, 공급 업체 정보 등) 
CREATE TABLE Products (
    UPC VARCHAR(20) PRIMARY KEY,
    Name VARCHAR(50) NOT NULL,
    Brand VARCHAR(50) NOT NULL,
    Price DECIMAL(10,2) NOT NULL,
    Vendor_ID INT NOT NULL,
    FOREIGN KEY (Vendor_ID) REFERENCES Vendor(ID)
);

-- 재고 관리 (각 매장에 어떤 제품이 얼마만큼 있는지) 
CREATE TABLE Inventory (
    Store_ID INT NOT NULL,
    UPC VARCHAR(20) NOT NULL,
    Quantity INT NOT NULL,
    MinimumStock INT NOT NULL,
    PRIMARY KEY (Store_ID, UPC),
    FOREIGN KEY (Store_ID) REFERENCES Stores(ID),
    FOREIGN KEY (UPC) REFERENCES Products(UPC)
);

-- 고객의 특정 매장에서 구매 이력 저장 (언제 구매했는지) 
CREATE TABLE SalesTransaction (
    ID INT PRIMARY KEY,
    Store_ID INT NOT NULL,
    SalesDate DATE NOT NULL,
    Customer_ID INT NOT NULL,
    FOREIGN KEY (Store_ID) REFERENCES Stores(ID),
    FOREIGN KEY (Customer_ID) REFERENCES Customer(ID)
);

-- 각 거래에서 어떤 제품이 몇 개 판매되었는지 
CREATE TABLE SalesItem (
    Transaction_ID INT NOT NULL,
    UPC VARCHAR(20) NOT NULL,
    Quantity INT NOT NULL,
    PRIMARY KEY (Transaction_ID, UPC),
    FOREIGN KEY (Transaction_ID) REFERENCES SalesTransaction(ID),
    FOREIGN KEY (UPC) REFERENCES Products(UPC)
);
