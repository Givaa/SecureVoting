import mysql.connector as sqlconnect

DataBase = sqlconnect.connect(
    host="localhost",
    user="root",
    password="Votazioneseggio2022!",
    database="Seggio"
)

Cursor = DataBase.cursor()

sql = "INSERT INTO Votante (RFID, BlockchainID, alreadyVoted) VALUES (%s, %s, %s)"
val = [("9A4405B1", "0xE3d211a67A67DD8705FbEE79D1C2Ac2445E20361", 0),
       ("8C35D337", "0xdE9809124d3B706F5fCa101c94B072aebCE184da", 0),
       ("6A750BB1", "0xDdDB0Ea14E9224F1c5B1C8152AB7903DB1995dAC", 0),
       ("FA4FF6B0", "0x19C3d2E69b6904B178fB105F7c366b761901A4a0", 0),
       ("83741AAA", "0x4a4418eeb4a05214E2A8BD298B176288983f3227", 0),
       ("233E92A9", "0xcbD2e874a33B31454f1f697677B65c682620E5DE", 0),
       ("436ECDA9", "0x8C07Beb67A8FD8FDD29953EFe145C0F88Ff0FF07", 0),
       ("13687EA9", "0xaCEFC97EE71E14dDC00e7c296Ae9aeD8969c0c1C", 0)]

Cursor.executemany(sql, val)
DataBase.commit()

print(Cursor.rowcount, "righe inserite!")

# disconnessione
DataBase.close()