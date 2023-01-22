import mysql.connector as sqlconnect

DataBase = sqlconnect.connect(
    host="localhost",
    user="root",
    password="Votazioneseggio2022!",
    database="Seggio"
)

Cursor = DataBase.cursor()

sql = "INSERT INTO Votante (RFID, BlockchainID, alreadyVoted) VALUES (%s, %s, %s)"
val = [("9A4405B1", "0x0D36eb93850ab7256ab4301468711455701F9792", 0),
       ("8C35D337", "0x4c72fD9A313E8C50bcE0E513b55D35f1568481da", 0),
       ("6A750BB1", "0x6A91A43c93Ac47128C2333f38d1b61a40A19dd1e", 0),
       ("FA4FF6B0", "0x6229b4a688A2f98F0a0B9E6ACA5AAF7b15511CbD", 0),
       ("83741AAA", "0x7d41d985c4f1acB0F3bdBA412DdD3FAE91eed0F9", 0),
       ("233E92A9", "0x852a0E667ec002B488289715D2609Dc61c8963fc", 0),
       ("436ECDA9", "0xbC329e1Fb244C52Fce7F0FFB3DCE81356BE93e19", 0),
       ("13687EA9", "0x6f26A87A04827aB28c80339D22c7B998B787d69C", 0)]

Cursor.executemany(sql, val)
DataBase.commit()

print(Cursor.rowcount, "righe inserite!")

# disconnessione
DataBase.close()