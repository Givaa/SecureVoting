import mysql.connector as sqlconnect

DataBase = sqlconnect.connect(
    host="localhost",
    user="root",
    password="Votazioneseggio2022!",
    database="Seggio"
)

Cursor = DataBase.cursor()

sql = "INSERT INTO Votante (RFID, BlockchainID, alreadyVoted) VALUES (%s, %s, %s)"
val = [("9A4405B1", "0x1e8feA34e827AA752d380B6f41B0264AB1449452", 0),
       ("8C35D337", "0x2830E8ab0a1a5AAeD2eA52d60eA33565064b1ef7", 0),
       ("6A750BB1", "0x7F210470279997A99CeeE4b77e02c3212AbC6245", 0),
       ("FA4FF6B0", "0xD3F2fe426975cdA3C2aaAf8352B9c410762f7190", 0),
       ("83741AAA", "0x7d41d985c4f1acB0F3bdBA412DdD3FAE91eed0F9", 0),
       ("233E92A9", "0x852a0E667ec002B488289715D2609Dc61c8963fc", 0),
       ("436ECDA9", "0xbC329e1Fb244C52Fce7F0FFB3DCE81356BE93e19", 0),
       ("13687EA9", "0x6f26A87A04827aB28c80339D22c7B998B787d69C", 0)]

Cursor.executemany(sql, val)
DataBase.commit()

print(Cursor.rowcount, "righe inserite!")

# disconnessione
DataBase.close()