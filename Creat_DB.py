import MySQLdb

db = MySQLdb.connect("localhost", "root" , "root", "framedata")

cursor = db.cursor()

cursor.execute("DROP TABLE IF EXISTS STARTUP")
cursor.execute("DROP TABLE IF EXISTS ACTIVE")
cursor.execute("DROP TABLE IF EXISTS RECOVERY")

sql = """CREATE TABLE  STARTUP(
		CL_LP INT ,
		CL_MP INT,
		CL_HP INT,
		CL_LK INT,
		CL_MK INT,
		CL_HK INT,
		S_LP INT, 
		S_MP INT,
		S_HP INT,
		S_LK INT,
		S_MK INT, 
		S_HK INT,
		CR_LP INT,
		CR_MP INT,
		CR_HP INT, 
		CR_LK INT,
		CR_MK INT,
		CR_HK INT,
		F_MP INT, 
		F_HP INT,
		FOCUS_ATTACK1 INT,
		FOCUS_ATTACK2 INT, 
		FOCUS_ATTACK3 INT,
		FORWARD_THROW INT,
		BACK_THROW INT,
		HADOKEN INT, 
		HADOKEN_EX INT, 
		SHORYUKEN_LP INT,
		SHORYUKEN_MP INT,
		SHORYUKEN_HP INT,
		SHORYUKEN_MP_HP INT, 
		SHORYUKEN_EX INT,
		HURRICANE_KICK_LK INT,
		HURRICANE_KICK_MK INT, 
		HURRICANE_KICK_HK INT,
		HURRICANE_KICK_EX INT,
		HURRICANE_KICK_LK_AIR INT, 
		HURRICANE_KICK_MK_AIR INT,
		HURRICANE_KICK_HK_AIR INT,
		HURRICANE_KICK_EX_AIR INT,
		SUPER_COMBO INT, 
		ULTRA_COMBO1 INT,
		ULTRA_COMBO2 INT
		)"""
		
sql_active = """CREATE TABLE  ACTIVE(
		CL_LP INT ,
		CL_MP INT,
		CL_HP INT,
		CL_LK INT,
		CL_MK INT,
		CL_HK INT,
		S_LP INT, 
		S_MP INT,
		S_HP INT,
		S_LK INT,
		S_MK INT, 
		S_HK INT,
		CR_LP INT,
		CR_MP INT,
		CR_HP INT, 
		CR_LK INT,
		CR_MK INT,
		CR_HK INT,
		F_MP INT, 
		F_HP INT,
		FOCUS_ATTACK1 INT,
		FOCUS_ATTACK2 INT, 
		FOCUS_ATTACK3 INT,
		FORWARD_THROW INT,
		BACK_THROW INT,
		HADOKEN INT, 
		HADOKEN_EX INT, 
		SHORYUKEN_LP INT,
		SHORYUKEN_MP INT,
		SHORYUKEN_HP INT,
		SHORYUKEN_MP_HP INT, 
		SHORYUKEN_EX INT,
		HURRICANE_KICK_LK INT,
		HURRICANE_KICK_MK INT, 
		HURRICANE_KICK_HK INT,
		HURRICANE_KICK_EX INT,
		HURRICANE_KICK_LK_AIR INT, 
		HURRICANE_KICK_MK_AIR INT,
		HURRICANE_KICK_HK_AIR INT,
		HURRICANE_KICK_EX_AIR INT,
		SUPER_COMBO INT, 
		ULTRA_COMBO1 INT,
		ULTRA_COMBO2 INT
		)"""

sql_recovery = """CREATE TABLE  RECOVERY(
		CL_LP INT ,
		CL_MP INT,
		CL_HP INT,
		CL_LK INT,
		CL_MK INT,
		CL_HK INT,
		S_LP INT, 
		S_MP INT,
		S_HP INT,
		S_LK INT,
		S_MK INT, 
		S_HK INT,
		CR_LP INT,
		CR_MP INT,
		CR_HP INT, 
		CR_LK INT,
		CR_MK INT,
		CR_HK INT,
		F_MP INT, 
		F_HP INT,
		FOCUS_ATTACK1 INT,
		FOCUS_ATTACK2 INT, 
		FOCUS_ATTACK3 INT,
		FORWARD_THROW INT,
		BACK_THROW INT,
		HADOKEN INT, 
		HADOKEN_EX INT, 
		SHORYUKEN_LP INT,
		SHORYUKEN_MP INT,
		SHORYUKEN_HP INT,
		SHORYUKEN_MP_HP INT, 
		SHORYUKEN_EX INT,
		HURRICANE_KICK_LK INT,
		HURRICANE_KICK_MK INT, 
		HURRICANE_KICK_HK INT,
		HURRICANE_KICK_EX INT,
		HURRICANE_KICK_LK_AIR INT, 
		HURRICANE_KICK_MK_AIR INT,
		HURRICANE_KICK_HK_AIR INT,
		HURRICANE_KICK_EX_AIR INT,
		SUPER_COMBO INT, 
		ULTRA_COMBO1 INT,
		ULTRA_COMBO2 INT
		)"""
		

		
cursor.execute(sql)
cursor.execute(sql_active)
cursor.execute(sql_recovery)

print "table successfully created"

sql_insert = """INSERT INTO STARTUP(CL_LP,
		CL_MP,
		CL_HP,
		CL_LK,
		CL_MK ,
		CL_HK ,
		S_LP , 
		S_MP ,
		S_HP ,
		S_LK ,
		S_MK , 
		S_HK ,
		CR_LP ,
		CR_MP ,
		CR_HP , 
		CR_LK ,
		CR_MK ,
		CR_HK ,
		F_MP , 
		F_HP ,
		FOCUS_ATTACK1 ,
		FOCUS_ATTACK2 , 
		FOCUS_ATTACK3 ,
		FORWARD_THROW ,
		BACK_THROW ,
		HADOKEN , 
		HADOKEN_EX , 
		SHORYUKEN_LP ,
		SHORYUKEN_MP ,
		SHORYUKEN_HP ,
		SHORYUKEN_MP_HP , 
		SHORYUKEN_EX ,
		HURRICANE_KICK_LK ,
		HURRICANE_KICK_MK , 
		HURRICANE_KICK_HK ,
		HURRICANE_KICK_EX ,
		HURRICANE_KICK_LK_AIR , 
		HURRICANE_KICK_MK_AIR ,
		HURRICANE_KICK_HK_AIR ,
		HURRICANE_KICK_EX_AIR ,
		SUPER_COMBO , 
		ULTRA_COMBO1 ,
		ULTRA_COMBO2 )VALUES(3,3,5,5,3,8,4,5,8,5,8,9,3,4,4,4,5,5,17,17,21,29,65,3,3,13,11,3,3,3,3,3,11,12,12,11,9,9,9,7,3,11,8)"""
		
sql_insert_active = """INSERT INTO ACTIVE(CL_LP,
		CL_MP,
		CL_HP,
		CL_LK,
		CL_MK ,
		CL_HK ,
		S_LP , 
		S_MP ,
		S_HP ,
		S_LK ,
		S_MK , 
		S_HK ,
		CR_LP ,
		CR_MP ,
		CR_HP , 
		CR_LK ,
		CR_MK ,
		CR_HK ,
		F_MP , 
		F_HP ,
		FOCUS_ATTACK1 ,
		FOCUS_ATTACK2 , 
		FOCUS_ATTACK3 ,
		FORWARD_THROW ,
		BACK_THROW ,
		HADOKEN , 
		HADOKEN_EX , 
		SHORYUKEN_LP ,
		SHORYUKEN_MP ,
		SHORYUKEN_HP ,
		SHORYUKEN_MP_HP , 
		SHORYUKEN_EX ,
		HURRICANE_KICK_LK ,
		HURRICANE_KICK_MK , 
		HURRICANE_KICK_HK ,
		HURRICANE_KICK_EX ,
		HURRICANE_KICK_LK_AIR , 
		HURRICANE_KICK_MK_AIR ,
		HURRICANE_KICK_HK_AIR ,
		HURRICANE_KICK_EX_AIR ,
		SUPER_COMBO , 
		ULTRA_COMBO1 ,
		ULTRA_COMBO2 )VALUES(3,3,7,5,5,64,3,4,3,6,2,4,2,4,8,3,5,4,2,4,2,2,2,2,2,0,0,14,24,14,0,24,24,10,10,5,6,6,6,5,0,0,63)"""
		
		
sql_insert_recovery = """INSERT INTO RECOVERY(CL_LP,
		CL_MP,
		CL_HP,
		CL_LK,
		CL_MK ,
		CL_HK ,
		S_LP , 
		S_MP ,
		S_HP ,
		S_LK ,
		S_MK , 
		S_HK ,
		CR_LP ,
		CR_MP ,
		CR_HP , 
		CR_LK ,
		CR_MK ,
		CR_HK ,
		F_MP , 
		F_HP ,
		FOCUS_ATTACK1 ,
		FOCUS_ATTACK2 , 
		FOCUS_ATTACK3 ,
		FORWARD_THROW ,
		BACK_THROW ,
		HADOKEN , 
		HADOKEN_EX , 
		SHORYUKEN_LP ,
		SHORYUKEN_MP ,
		SHORYUKEN_HP ,
		SHORYUKEN_MP_HP , 
		SHORYUKEN_EX ,
		HURRICANE_KICK_LK ,
		HURRICANE_KICK_MK , 
		HURRICANE_KICK_HK ,
		HURRICANE_KICK_EX ,
		HURRICANE_KICK_LK_AIR , 
		HURRICANE_KICK_MK_AIR ,
		HURRICANE_KICK_HK_AIR ,
		HURRICANE_KICK_EX_AIR ,
		SUPER_COMBO , 
		ULTRA_COMBO1 ,
		ULTRA_COMBO2 )VALUES(6,21,26,7,16,17,6,14,15,6,14,15,6,17,20,7,8,28,9,12,28,14,18,35,35,35,20,20,45,40,0,0,0,0,0,0,0,0,0,0,52,120,0)"""

try:
	cursor.execute(sql_insert)
	cursor.execute(sql_insert_active)
	cursor.execute(sql_insert_recovery)
	
	db.commit()
	print "successful commit"
except:
	db.rollback()
	print "error inserting into database"	
	
cursor.execute("SELECT VERSION()")
data = cursor.fetchone()

print "Database version: %s " % data

sql_find = "SELECT * FROM STARTUP"

sql_find_ACTIVE = "SELECT * FROM ACTIVE"

sql_find_RECOVERY = "SELECT * FROM RECOVERY"


try:
	cursor.execute(sql_find)
	#cursor.execute(sql_find_ACTIVE)
	results = cursor.fetchall()
	
	
	for row in results:
		CL_LP = row[0]
		CL_MP = row[1]
		CL_HP = row[2]
		CL_LK = row[3]
		CL_MK = row[4]
		CL_HK = row[5]
		S_LP  = row[6]
		S_MP = row[7]
		S_HP = row[8]
		S_LK = row[9]
		S_MK = row[10]
		S_HK = row[11]
		CR_LP = row[12]
		CR_MP = row[13]
		CR_HP = row[14]
		CR_LK = row[15]
		CR_MK = row[16]
		CR_HK = row[17]
		F_MP  = row[18]
		F_HP = row[19]
		FOCUS_ATTACK1 = row[20]
		FOCUS_ATTACK2 = row[21]
		FOCUS_ATTACK3 = row[22]
		FORWARD_THROW = row[23]
		BACK_THROW = row[24]
		HADOKEN = row[25]
		HADOKEN_EX  = row[26]
		SHORYUKEN_LP = row[27]
		SHORYUKEN_MP = row[28]
		SHORYUKEN_HP = row[29]
		SHORYUKEN_MP_HP = row[30]
		SHORYUKEN_EX = row[31]
		HURRICANE_KICK_LK = row[32]
		HURRICANE_KICK_MK  = row[33]
		HURRICANE_KICK_HK = row[34]
		HURRICANE_KICK_EX = row[35]
		HURRICANE_KICK_LK_AIR = row[36]
		HURRICANE_KICK_MK_AIR = row[37]
		HURRICANE_KICK_HK_AIR = row[38]
		HURRICANE_KICK_EX_AIR = row[39]
		SUPER_COMBO  = row[40]
		ULTRA_COMBO1 = row[41]
		
		print "CL_LP=%d,CL_MP=%d,CL_HP=%d,CL_LK=%d,CL_MK=%d,CL_HK=%d,S_LP=%d,S_MP=%d,S_HP=%d,S_LK=%d,S_MK=%d,S_HK=%d,CR_LP=%d,CR_MP=%d,CR_HP=%d,CR_LK=%d,CR_MK=%d,CR_HK=%d,F_MP=%d,F_HP=%d,FOCUS_ATTACK1=%d,FOCUS_ATTACK2=%d,FOCUS_ATTACK3=%d,FORWARD_THROW=%d,BACK_THROW=%d,HADOKEN=%d,HADOKEN_EX=%d,SHORYUKEN_LP=%d,SHORYUKEN_MP=%d,SHORYUKEN_HP=%d,SHORYUKEN_MP_HP=%d,SHORYUKEN_EX=%d,HURRICANE_KICK_LK=%d,HURRICANE_KICK_MK=%d,HURRICANE_KICK_HK=%d,HURRICANE_KICK_EX=%d,HURRICANE_KICK_LK_AIR=%d,HURRICANE_KICK_MK_AIR=%d,HURRICANE_KICK_HK_AIR=%d,HURRICANE_KICK_EX_AIR=%d,SUPER_COMBO=%d,ULTRA_COMBO1=%d " % \
		(CL_LP,CL_MP,CL_HP,CL_LK,CL_MK,CL_HK,S_LP,S_MP,S_HP,S_LK,S_MK,S_HK,CR_LP,CR_MP,CR_HP,CR_LK,CR_MK,CR_HK,F_MP,F_HP,FOCUS_ATTACK1,FOCUS_ATTACK2,
		FOCUS_ATTACK3,FORWARD_THROW,BACK_THROW,HADOKEN,HADOKEN_EX,SHORYUKEN_LP,SHORYUKEN_MP,SHORYUKEN_HP,SHORYUKEN_MP_HP,SHORYUKEN_EX,HURRICANE_KICK_LK,
		HURRICANE_KICK_MK,HURRICANE_KICK_HK,HURRICANE_KICK_EX,HURRICANE_KICK_LK_AIR,HURRICANE_KICK_MK_AIR,HURRICANE_KICK_HK_AIR,HURRICANE_KICK_EX_AIR,
		SUPER_COMBO,ULTRA_COMBO1
		)
		print "\n"
	
		
except:	
	print "error: unable tp fetch data"
	
try:
	#cursor.execute(sql_find)
	cursor.execute(sql_find_ACTIVE)
	results = cursor.fetchall()
	
	
	for row in results:
		CL_LP_ACT = row[0]
		CL_MP_ACT = row[1]
		CL_HP_ACT = row[2]
		CL_LK_ACT = row[3]
		CL_MK_ACT = row[4]
		CL_HK_ACT = row[5]
		S_LP_ACT  = row[6]
		S_MP_ACT = row[7]
		S_HP_ACT = row[8]
		S_LK_ACT = row[9]
		S_MK_ACT = row[10]
		S_HK_ACT = row[11]
		CR_LP_ACT = row[12]
		CR_MP_ACT = row[13]
		CR_HP_ACT = row[14]
		CR_LK_ACT = row[15]
		CR_MK_ACT = row[16]
		CR_HK_ACT = row[17]
		F_MP_ACT  = row[18]
		F_HP_ACT = row[19]
		FOCUS_ATTACK1_ACT = row[20]
		FOCUS_ATTACK2_ACT = row[21]
		FOCUS_ATTACK3_ACT = row[22]
		FORWARD_THROW_ACT = row[23]
		BACK_THROW_ACT = row[24]
		HADOKEN_ACT = row[25]
		HADOKEN_EX_ACT  = row[26]
		SHORYUKEN_LP_ACT = row[27]
		SHORYUKEN_MP_ACT = row[28]
		SHORYUKEN_HP_ACT = row[29]
		SHORYUKEN_MP_HP_ACT = row[30]
		SHORYUKEN_EX_ACT = row[31]
		HURRICANE_KICK_LK_ACT = row[32]
		HURRICANE_KICK_MK_ACT  = row[33]
		HURRICANE_KICK_HK_ACT = row[34]
		HURRICANE_KICK_EX_ACT = row[35]
		HURRICANE_KICK_LK_AIR_ACT = row[36]
		HURRICANE_KICK_MK_AIR_ACT = row[37]
		HURRICANE_KICK_HK_AIR_ACT = row[38]
		HURRICANE_KICK_EX_AIR_ACT = row[39]
		SUPER_COMBO_ACT  = row[40]
		ULTRA_COMBO1_ACT = row[41]
		
		print "CL_LP=%d,CL_MP=%d,CL_HP=%d,CL_LK=%d,CL_MK=%d,CL_HK=%d,S_LP=%d,S_MP=%d,S_HP=%d,S_LK=%d,S_MK=%d,S_HK=%d,CR_LP=%d,CR_MP=%d,CR_HP=%d,CR_LK=%d,CR_MK=%d,CR_HK=%d,F_MP=%d,F_HP=%d,FOCUS_ATTACK1=%d,FOCUS_ATTACK2=%d,FOCUS_ATTACK3=%d,FORWARD_THROW=%d,BACK_THROW=%d,HADOKEN=%d,HADOKEN_EX=%d,SHORYUKEN_LP=%d,SHORYUKEN_MP=%d,SHORYUKEN_HP=%d,SHORYUKEN_MP_HP=%d,SHORYUKEN_EX=%d,HURRICANE_KICK_LK=%d,HURRICANE_KICK_MK=%d,HURRICANE_KICK_HK=%d,HURRICANE_KICK_EX=%d,HURRICANE_KICK_LK_AIR=%d,HURRICANE_KICK_MK_AIR=%d,HURRICANE_KICK_HK_AIR=%d,HURRICANE_KICK_EX_AIR=%d,SUPER_COMBO=%d,ULTRA_COMBO1=%d " % \
		(CL_LP_ACT,CL_MP_ACT,CL_HP_ACT,CL_LK_ACT,CL_MK_ACT,CL_HK_ACT,S_LP_ACT,S_MP_ACT,S_HP_ACT,S_LK_ACT,S_MK_ACT,S_HK_ACT,CR_LP_ACT,CR_MP_ACT,CR_HP_ACT,CR_LK_ACT,CR_MK_ACT,CR_HK_ACT,F_MP_ACT,F_HP_ACT,FOCUS_ATTACK1_ACT,FOCUS_ATTACK2_ACT,
		FOCUS_ATTACK3_ACT,FORWARD_THROW_ACT,BACK_THROW_ACT,HADOKEN_ACT,HADOKEN_EX_ACT,SHORYUKEN_LP_ACT,SHORYUKEN_MP_ACT,SHORYUKEN_HP_ACT,SHORYUKEN_MP_HP_ACT,SHORYUKEN_EX_ACT,HURRICANE_KICK_LK_ACT,
		HURRICANE_KICK_MK_ACT,HURRICANE_KICK_HK_ACT,HURRICANE_KICK_EX_ACT,HURRICANE_KICK_LK_AIR_ACT,HURRICANE_KICK_MK_AIR_ACT,HURRICANE_KICK_HK_AIR_ACT,HURRICANE_KICK_EX_AIR_ACT,
		SUPER_COMBO_ACT,ULTRA_COMBO1_ACT
		)
		print "\n"
except:	
	print "error: unable tp fetch data"
	
try:
	#cursor.execute(sql_find)
	cursor.execute(sql_find_RECOVERY)
	results = cursor.fetchall()
	
	
	for row in results:
		CL_LP_REC = row[0]
		CL_MP_REC = row[1]
		CL_HP_REC = row[2]
		CL_LK_REC = row[3]
		CL_MK_REC = row[4]
		CL_HK_REC = row[5]
		S_LP_REC  = row[6]
		S_MP_REC = row[7]
		S_HP_REC = row[8]
		S_LK_REC = row[9]
		S_MK_REC = row[10]
		S_HK_REC = row[11]
		CR_LP_REC = row[12]
		CR_MP_REC = row[13]
		CR_HP_REC = row[14]
		CR_LK_REC = row[15]
		CR_MK_REC = row[16]
		CR_HK_REC = row[17]
		F_MP_REC  = row[18]
		F_HP_REC = row[19]
		FOCUS_ATTACK1_REC = row[20]
		FOCUS_ATTACK2_REC = row[21]
		FOCUS_ATTACK3_REC = row[22]
		FORWARD_THROW_REC = row[23]
		BACK_THROW_REC = row[24]
		HADOKEN_REC = row[25]
		HADOKEN_EX_REC  = row[26]
		SHORYUKEN_LP_REC = row[27]
		SHORYUKEN_MP_REC = row[28]
		SHORYUKEN_HP_REC = row[29]
		SHORYUKEN_MP_HP_REC = row[30]
		SHORYUKEN_EX_REC = row[31]
		HURRICANE_KICK_LK_REC = row[32]
		HURRICANE_KICK_MK_REC  = row[33]
		HURRICANE_KICK_HK_REC = row[34]
		HURRICANE_KICK_EX_REC = row[35]
		HURRICANE_KICK_LK_AIR_REC = row[36]
		HURRICANE_KICK_MK_AIR_REC = row[37]
		HURRICANE_KICK_HK_AIR_REC = row[38]
		HURRICANE_KICK_EX_AIR_REC = row[39]
		SUPER_COMBO_REC  = row[40]
		ULTRA_COMBO1_REC = row[41]
		
		print "CL_LP=%d,CL_MP=%d,CL_HP=%d,CL_LK=%d,CL_MK=%d,CL_HK=%d,S_LP=%d,S_MP=%d,S_HP=%d,S_LK=%d,S_MK=%d,S_HK=%d,CR_LP=%d,CR_MP=%d,CR_HP=%d,CR_LK=%d,CR_MK=%d,CR_HK=%d,F_MP=%d,F_HP=%d,FOCUS_ATTACK1=%d,FOCUS_ATTACK2=%d,FOCUS_ATTACK3=%d,FORWARD_THROW=%d,BACK_THROW=%d,HADOKEN=%d,HADOKEN_EX=%d,SHORYUKEN_LP=%d,SHORYUKEN_MP=%d,SHORYUKEN_HP=%d,SHORYUKEN_MP_HP=%d,SHORYUKEN_EX=%d,HURRICANE_KICK_LK=%d,HURRICANE_KICK_MK=%d,HURRICANE_KICK_HK=%d,HURRICANE_KICK_EX=%d,HURRICANE_KICK_LK_AIR=%d,HURRICANE_KICK_MK_AIR=%d,HURRICANE_KICK_HK_AIR=%d,HURRICANE_KICK_EX_AIR=%d,SUPER_COMBO=%d,ULTRA_COMBO1=%d " % \
		(CL_LP_REC,CL_MP_REC,CL_HP_REC,CL_LK_REC,CL_MK_REC,CL_HK_REC,S_LP_REC,S_MP_REC,S_HP_REC,S_LK_REC,S_MK_REC,S_HK_REC,CR_LP_REC,CR_MP_REC,CR_HP_REC,CR_LK_REC,CR_MK_REC,CR_HK_REC,F_MP_REC,F_HP_REC,FOCUS_ATTACK1_REC,FOCUS_ATTACK2_REC,
		FOCUS_ATTACK3_REC,FORWARD_THROW_REC,BACK_THROW_REC,HADOKEN_REC,HADOKEN_EX_REC,SHORYUKEN_LP_REC,SHORYUKEN_MP_REC,SHORYUKEN_HP_REC,SHORYUKEN_MP_HP_REC,SHORYUKEN_EX_REC,HURRICANE_KICK_LK_REC,
		HURRICANE_KICK_MK_REC,HURRICANE_KICK_HK_REC,HURRICANE_KICK_EX_REC,HURRICANE_KICK_LK_AIR_REC,HURRICANE_KICK_MK_AIR_REC,HURRICANE_KICK_HK_AIR_REC,HURRICANE_KICK_EX_AIR_REC,
		SUPER_COMBO_REC,ULTRA_COMBO1_REC
		)
except:	
	print "error: unable tp fetch data"
	
query = raw_input ("enter wanted frame data\n")
print query
sql_find_particular = 'SELECT %s FROM STARTUP' % query
#in_p  =', '.join(list(map(lambda arg:  "'%s'" % query)))
print ("%s",sql_find_particular)
#sql_find_particular = sql_find_particular % in_p

try:
	#cursor.execute(sql_find)
	cursor.execute(sql_find_particular)
	results = cursor.fetchone()
	
	for result in results:
		print result
	
except:	
	print "error: unable tp fetch data"
	
	

#print "frame = %d " % \
#()


		
db.close()