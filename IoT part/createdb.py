import mysql.connector as sqlconnect

DataBase = sqlconnect.connect(
    host="localhost",
    user="root",
    password="Votazioneseggio2022!"
)

Cursor = DataBase.cursor()

Cursor.execute("DROP DATABASE IF EXISTS Seggio")

Cursor.execute("CREATE DATABASE Seggio")
print("Database creato!")

Cursor.execute(("USE Seggio"))

TableName = "CREATE TABLE Votante ( RFID VARCHAR(8), BlockchainID VARCHAR(42), alreadyVoted Bit(1));"

Cursor.execute(TableName)
print("Tabella Votante creata!")