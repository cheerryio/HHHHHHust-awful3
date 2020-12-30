
infoMenuInstructions byte "0.logout",0AH,0DH,	\
						 "1.login",0AH,0DH,	\
						 "2.find good and put in cargo",0AH,0DH,	\
						 "3.order good",0AH,0DH,	\
						 "4.count all recommend index",0AH,0DH,	\
						 "5.---",0AH,0DH,
						 "6.modify good info(boss)",0AH,0DH,
						 "7.",0AH,0DH,
						 "8.show registrater",0AH,0DH,	\
						 0

infoInputChoice byte "input your choice",0
infoInputName byte "input your name",0
infoInputPassword byte "input your password",0
infoInputNameError byte "wrong format of name",0
infoInputPasswordError byte "wroing format of password",0
infoLoginSuccess byte "login success",0
infoLoginFail byte "login fail",0

infoInputGoodName byte "input good name",0
infoGoodNameError byte "good name format error",0
infoFindGood byte "find good and store it in cargo",0
infoDontFindGood byte "fail to find good",0
infoUpdateRecommendIndex byte "successfully count all goods recommend index",0