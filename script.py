import sqlite3

conn = sqlite3.connect('database.db')  
c = conn.cursor() 

# Create table - user
c.execute("CREATE TABLE IF NOT EXISTS user (email TEXT, nickName TEXT, password TEXT,isActive BIT, CONSTRAINT PK_NickName PRIMARY KEY (nickName))")
c.execute("INSERT INTO user (email, nickName, password, isActive) VALUES('email', 'nickname', 'password' , 1)")  
c.execute("INSERT INTO user (email, nickName, password, isActive) VALUES('email1', 'nickname1', 'password1' , 1)")  
conn.commit()
