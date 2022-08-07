#include "classic.bi"

10 PRINT TAB(33);"LUNAR"
20 PRINT TAB(15);"CREATIVE COMPUTING MORRISTOWN, NEW JERSEY" 
25 PRINT:PRINT:PRINT
30 PRINT "THIS IS A COMPUTER SIMULATION OF AN APOLLO LUNAR" 
40 PRINT "LANDING CAPSULE.": PRINT: PRINT
50 PRINT "THE ON-BOARD COMPUTER HAS FAILED (IT WAS MADE BY" 
60 PRINT "XEROX) SO YOU HAVE TO LAND THE CAPSULE MANUALLY."
70 PRINT: PRINT "SET BURN RATE OF RETRO ROCKETS TO ANY VALUE BETWEEN" 
80 PRINT "0 (FREE FALL) AND 200 (MAXIMUM BURN) POUNDS PER SECOND." 
90 PRINT "SET NEW BURN RATE EVERY 10 SECONDS.": PRINT 
100 PRINT "CAPSULE WEIGHT 32,500 LBS; FUEL WEIGHT 16,500 LBS."
110 PRINT: PRINT: PRINT: PRINT "GOOD LUCK"
120 L=0
130 PRINT: PRINT "SEC","MI + FT","","MPH","LB FUEL","BURN RATE":PRINT 
140 A=120:V=1:M=33000:N=16500:G=1E-03:Z=1.8
150 PRINT L,INT(A);" +  ";INT(5280*(A-INT(A))),3600*V,M-N,"? ";:INPUT K:T=10 
160 IF M-N<1E-03 THEN 240
170 IF T<1E-03 THEN 150
180 S=T: IF M>=N+S*K THEN 200
190 S=(M-N)/K
200 GOSUB 420: IF I<=0 THEN 340
210 IF V<=0 THEN 230
220 IF J<0 THEN 370
230 GOSUB 330: GOTO 160
240 PRINT "FUEL OUT AT";L;"SECONDS":S=(-V+SQR(V*V+2*A*G))/G
250 V=V+G*S: L=L+S
260 W=3600*V: PRINT "ON MOON AT ";L;" SECONDS - IMPACT VELOCITY ";W;" MPH" 
274 IF W<=1.2 THEN PRINT "PERFECT LANDING!": GOTO 440 
280 IF W<=10 THEN PRINT "GOOD LANDING (COULD RE BETTER)":GOTO 440 
282 IF W>60 THEN 300
284 PRINT "CRAFT DAMAGE... YOU'RE STRANDED HERE UNTIL A RESCUE" 
286 PRINT "PARTY ARRIVES. HOPE YOU HAVE ENOUGH OXYGEN!" 
288 GOTO 440
300 PRINT "SORRY THERE NERE NO SURVIVORS. YOU BLOW IT!"
310 PRINT "IN FACT, YOU BLASTED A NEW LUNAR CRATER ";W*.227;" FEET DEEP!"
320 GOTO 440
330 L=L+S: T=T-S: M=M-S*K: A=I: V=J: RETURN
340 IF S<5E-03 THEN 260
350 D=V+SQR(V*V+2*A*(G-Z*K/M)):S=2*A/D
360 GOSUB 420: GOSUB 330: GOTO 340
370 W=(1-M*G/(Z*K))/2: S=M*V/(Z*K*(W+SQR(W*W+V/Z)))+.05:GOSUB 420
380 IF I<=0 THEN 340
390 GOSUB 330: IF J>0 THEN 160
400 IF V>0 THEN 370
410 GOTO 160
420 Q=S*K/M: J=V+G*S+Z*(-Q-Q*Q/2-(Q*Q*Q)/3-(Q*Q*Q*Q)/4-(Q*Q*Q*Q*Q)/5)
430 I=A-G*S*S/2-V*S+Z*S*(Q/2+(Q*Q)/6+(Q*Q*Q)/12+(Q*Q*Q*Q)/20+(Q*Q*Q*Q*Q)/30):RETURN
440 PRINT:PRINT:PRINT:PRINT "TRY AGAIN?? ": GOTO 70
