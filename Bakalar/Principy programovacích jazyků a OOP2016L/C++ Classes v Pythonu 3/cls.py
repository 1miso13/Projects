#!/usr/bin/python

#CLS:xondre08
import keyword
import sys
import re
import os.path
import xml.etree.ElementTree as ET
import xml.dom
from lxml import etree
#pomocne premenne
debug=0

firstArg=0
inputZadany=0
outputZadany=0
prettyZadany=0
detailsZadany=0
searchZadany=0
conflictsZadany=0
err=0
token=""
stringToParse=""
position=0
endOfString=0
tokenTYPE=""
actualModifier="private";
header="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
#pomocne funkcie

#je identifikator 
def isIdentifier(string):
	return re.match("[_A-Za-z][_a-zA-Z0-9]*$",string) and not keyword.iskeyword(string)

def zmazMedzeruPredAZa(string):
	return re.sub(r'^ | $','',string)
	
#inputFile
def getInput(is_set):
	global err
	string=""
	if is_set==1:
		if os.path.isfile(inputFile):
			try:
				inputfileI = open(inputFile, "r", encoding="utf-8")
				string = inputfileI.read()
				inputfileI.close()
			except OSError:
				err=1
		else:
			print("Vstupny subor neexistuje", file=sys.stderr)
			err=1
	else:
		inputfileI = sys.stdin
		string=inputfileI.read()
	return string
	
def indent(elem,K, level=0):
	i = "\n" + level*K*" "
	if len(elem):
		if not elem.text or not elem.text.strip():
			elem.text = i + K*" "
		if not elem.tail or not elem.tail.strip():
			elem.tail = i
		for elem in elem:
			indent(elem,K, level+1)
		if not elem.tail or not elem.tail.strip():
			elem.tail = i
	else:
		if level and (not elem.tail or not elem.tail.strip()):
			elem.tail = i

#outputFile
def outPut(stringToPut,is_set):
	if is_set==1:
		try:
			ouputfileI = open(outputFile, "w+")#, encoding="utf-8"
			string = ouputfileI.write(stringToPut)
			ouputfileI.close()
		except OSError:
			err=1
	else:
		print(stringToPut, end='')
		# ouputfileI = sys.stdout
		# string = ouputfileI.write(stringToPut)

#lexikalna analiza
def setLexicalString(str):
	global stringToParse
	global position
	global endOfString
	stringToParse=str
	position=0
	endOfString=0


def pushBackChar():
	global position
	position=position-1

def nextChar():
	global position
	if position>=len(stringToParse):
		znak="EOF"
	else:
		znak=stringToParse[position]
		position=position+1
	return znak


def nextTokena():
	global endOfString
	global tokenTYPE
	token=""
	state=0;
	while True:
		c=nextChar()
		if c=="EOF":
			endOfString=1
			return token
		elif state==0:
			if re.match(r'[a-zA-z_]',c):
				token+=c
				state=1
			elif re.match(r'[0-9]',c):
				token+=c
				state=2
			elif re.match(r':',c):
				token+=c
				state=3
			#prazdne znaky preskakujeme
			elif re.match(r'\s',c):
				continue
			elif c=="\"":
				state=4
				token+=c
			else:#spacial
				token=c
				tokenTYPE="special"
				return token
		#string
		elif state==1:
			if re.match(r'\w',c):
				token+=c
			else:
				pushBackChar()
				tokenTYPE="string"
				return token
		#number
		elif state==2:
			if re.match(r'[0-9]',c):
				token+=c
			if re.match(r'[a-zA-z_]',c):
				err=1
			else:
				pushBackChar()
				tokenTYPE="num"
				return token
		#:: or :
		elif state==3:
			tokenTYPE="special"
			if re.match(r':',c):
				token+=c
				return token
			else:
				pushBackChar()
				return token
		#"text"
		elif state==4:
			if c=="\"":
				token+=c
				tokenTYPE="text"
				return token
			else:
				token+=c

#pomocna funkcia na vypis vstupu
def nextToken():
	token = nextTokena()
	# if debug==1:
		# print("<<<<<<<<<<<#"+token+"#>>>>>>>>>>>>>", file=sys.stderr)
	return token

#pomocna funkcia na spojenie jednotlivych typov do jednej premennej(stringu)
def spojTypy(types):
	t=types[0]
	for idx, s in enumerate(types):
		if idx!=0:
			t+=" "+s
		
	return t

#test ci este nebola zadana dana trieda
def testIfUniqueClass(classname):
	global list
	alreadySet=0
	for d in list:
		if d["Name"]==classname:
			alreadySet=1
	return alreadySet
#overi ci je spravny typ premennej/metody
#ak nieje tak treba skontrolovat 
#ci sa namiesto standardneho typu nenachadza trieda
#type - pole typov
#vracia uspesnost
def testIfValidType(types):
	typy=spojTypy(types)
	# if debug==1:
		# print("@@@@@@@@@@#"+typy+"#@@@@@@@@@@@@", file=sys.stderr)
	
	return re.match(r'(void|char|unsigned char|signed char|int|unsigned int|signed int|short int|unsigned short int|signed short int|long int|signed long int|unsigned long int|float|double|long double|wchar_t|bool)(\s(\**|&))?',typy) or  testIfUniqueClass(types[0].split(" ",1)[0])==1

def saveMethod(methodName,methodType,scope,arguments,virual,fromClass="0",printable="yes"):
	global ArrayOfPublicMethods
	global ArrayOfPrivateMethods
	global ArrayOfProtectedMethods
	global actualModifier
	
	if actualModifier=="public":
		ArrayOfPublicMethods.append({"Name":methodName,"Type":spojTypy(methodType),"Scope":scope,"Arguments":arguments,"Virtual":virual,"From":fromClass,"printable":printable})
	elif actualModifier=="private":
		ArrayOfPrivateMethods.append({"Name":methodName,"Type":spojTypy(methodType),"Scope":scope,"Arguments":arguments,"Virtual":virual,"From":fromClass,"printable":printable})
	elif actualModifier=="protected":
		ArrayOfProtectedMethods.append({"Name":methodName,"Type":spojTypy(methodType),"Scope":scope,"Arguments":arguments,"Virtual":virual,"From":fromClass,"printable":printable})
	

def saveVariable(varName,varType,scope,fromClass="0",printable="yes"):
	global ArrayOfPublicVariables
	global ArrayOfPrivateVariables
	global ArrayOfProtectedVariables
	global actualModifier
	if actualModifier=="public":
		ArrayOfPublicVariables.append({"Name":varName,"Type":spojTypy(varType),"Scope":scope,"From":fromClass,"printable":printable})
	elif actualModifier=="private":
		ArrayOfPrivateVariables.append({"Name":varName,"Type":spojTypy(varType),"Scope":scope,"From":fromClass,"printable":printable})
	elif actualModifier=="protected":
		ArrayOfProtectedVariables.append({"Name":varName,"Type":spojTypy(varType),"Scope":scope,"From":fromClass,"printable":printable})
	

#vyhlada metody a premenne s rovnakym menom a bud vyhodi chybu alebo odstrani
def vyhladajKonflikt(vkladanyMeno,vkladanyTyp,isVar=False):
	global ArrayOfPublicVariables
	global ArrayOfPrivateVariables
	global ArrayOfProtectedVariables
	global ArrayOfPublicMethods
	global ArrayOfPrivateMethods
	global ArrayOfProtectedMethods
	
	
	if isVar == True:
		for s in ArrayOfPublicVariables[:]:
			porovnajVar(vkladanyMeno,vkladanyTyp,s,ArrayOfPublicVariables)
		for s in ArrayOfPrivateVariables[:] :
			porovnajVar(vkladanyMeno,vkladanyTyp,s,ArrayOfPrivateVariables)
		for s in ArrayOfProtectedVariables[:]:
			porovnajVar(vkladanyMeno,vkladanyTyp,s,ArrayOfProtectedVariables)
		
		for s in ArrayOfPublicMethods[:]:
			porovnajVar(vkladanyMeno,vkladanyTyp,s,ArrayOfPublicMethods,False)
		for s in  ArrayOfPrivateMethods:
			porovnajVar(vkladanyMeno,vkladanyTyp,s,ArrayOfPrivateMethods,False)
		for s in  ArrayOfProtectedMethods:
			porovnajVar(vkladanyMeno,vkladanyTyp,s,ArrayOfProtectedMethods,False)
	else:
		for s in ArrayOfPublicVariables[:] :
			porovnajVar(vkladanyMeno,vkladanyTyp,s,ArrayOfPublicVariables,False)
		for s in ArrayOfPrivateVariables[:] :
			porovnajVar(vkladanyMeno,vkladanyTyp,s,ArrayOfPrivateVariables,False)
		for s in ArrayOfProtectedVariables[:]:
			porovnajVar(vkladanyMeno,vkladanyTyp,s,ArrayOfProtectedVariables,False)
		
		for s in ArrayOfPublicMethods[:]:
			porovnajVar(vkladanyMeno,vkladanyTyp,s,ArrayOfPublicMethods)
		for s in  ArrayOfPrivateMethods[:]:
			porovnajVar(vkladanyMeno,vkladanyTyp,s,ArrayOfPrivateMethods)
		for s in  ArrayOfProtectedMethods[:]:
			porovnajVar(vkladanyMeno,vkladanyTyp,s,ArrayOfProtectedMethods)
	
	
	

def porovnajVar(vkladanyMeno,vkladanyTyp,s,array,T=True):
	global using
	
	if vkladanyMeno==s["Name"]:
		if T:
			if vkladanyTyp==s["Type"]:
				if debug==1:
					print("Odstranujem "+s["Name"]+" "+s["Type"]+" z \""+s["From"], file=sys.stderr)
				odstran(s,array)
			else:
				#chyba
				print("nastala kolizia "+s["Name"]+" "+s["Type"]+" z \""+s["From"]+"\" a "+vkladanyMeno+" "+vkladanyTyp, file=sys.stderr)
				sys.exit(21)
		else:
			print("nastala kolizia "+s["Name"]+" "+s["Type"]+" z \""+s["From"]+"\" a "+vkladanyMeno+" "+vkladanyTyp+".Jedno je metoda a druhe Premenna.", file=sys.stderr)
			sys.exit(21)
def odstran(s,array):
	
	array.remove(s)
	

def tryToFind(item,N,array):
	global ArrayOfPublicVariables
	global ArrayOfPrivateVariables
	global ArrayOfProtectedVariables
	global ArrayOfPublicMethods
	global ArrayOfPrivateMethods
	global ArrayOfProtectedMethods
	tryToFindHelpFunc(item,ArrayOfPublicVariables,N,array)
	tryToFindHelpFunc(item,ArrayOfPrivateVariables,N,array)
	tryToFindHelpFunc(item,ArrayOfProtectedVariables,N,array)
	tryToFindHelpFunc(item,ArrayOfPublicMethods,N,array)
	tryToFindHelpFunc(item,ArrayOfPrivateMethods,N,array)
	tryToFindHelpFunc(item,ArrayOfProtectedMethods,N,array)


def tryToFindHelpFunc(item,array,N,array2):
	global using
	#print("\n\n\n"+str(N)+" "+str(item))
	for index,itemA in enumerate(array):
		#print(str(index)+" "+str(itemA))
		if ( id(array)!=id(array2) or index!=N ) and item["Name"]==itemA["Name"]:
			# if item["From"]==itemA["From"]:
				# odstranime zo znoznamu jednu kedze su rovnake
				# array.remove(item)
			# else:
			print("nastala kolizia "+item["Name"]+" z \""+item["From"]+"\" a "+itemA["Name"]+" z \""+itemA["From"], file=sys.stderr)
			sys.exit(21)
#parsovanie argumentov


for arg in sys.argv:
	#preskocenie prveho argumentu
	if firstArg==0:
		firstArg=1
	else:
		if len(sys.argv)==2 and ((arg=='--help') or (arg=='-h')):
			print('helphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelphelp')
			sys.exit(0)
		elif re.match(r'--input=.+',arg)and inputZadany==0:
			inputZadany=1
			inputFile=arg.split('=', 1 )[1] 
		elif re.match(r'--output=.+',arg) and outputZadany==0:
			outputZadany=1
			outputFile=arg.split('=', 1 )[1] 
		elif re.match(r'--pretty-xml=.+',arg)and prettyZadany==0:
			prettyZadany=2
			if arg.split('=', 1 )[1].isdigit():
				prettyXmlK=int(arg.split('=', 1 )[1])
			else:
				print("Zly pocet argumentov", file=sys.stderr)
				sys.exit(1)
		elif '--pretty-xml'==arg and prettyZadany==0:
			prettyZadany=1
		elif re.match(r'--details=.+',arg)and detailsZadany==0:
			detailsZadany=1
			detailsClass=arg.split('=', 1 )[1] 
		elif re.match(r'--details',arg)and detailsZadany==0:
			detailsZadany=2
		elif re.match(r'--search=.*',arg) and searchZadany==0:
			searchZadany=1
			searchXpatch=arg.split('=', 1 )[1] 
		elif arg=="--conflicts":
			conflictsZadany=1
		else:
			#chyba
			print("Zly pocet argumentovqwert", file=sys.stderr)
			sys.exit(1)

#pomocny vypis
if debug==1:
	print('Arguments:', file=sys.stderr)
	print('\tInput:', file=sys.stderr)
	if inputZadany==1:
		print("\t\tfile: "+inputFile, file=sys.stderr)
	else:
		print("\t\tfrom standard input.", file=sys.stderr)
	print('\tOutput:', file=sys.stderr)
	if outputZadany==1:
		print("\t\tfile: "+outputFile, file=sys.stderr)
	else:
		print("\t\tto standard output.", file=sys.stderr)
	if prettyZadany!=0:
		print('\tpretty-xml:', file=sys.stderr)
		if prettyZadany==2:
			print('\t\tchanged to '+str(prettyXmlK), file=sys.stderr)
		else:
			print('\t\tdefault 4', file=sys.stderr)
	if detailsZadany!=0:
		print('\tDetails:', file=sys.stderr)
		if detailsZadany==1:
			print('\t\tclass:'+detailsClass, file=sys.stderr)
		if detailsZadany==2:
			print('\t\tclass: all classes', file=sys.stderr)
	if searchZadany==1:
		print('\tSearch by XPATH:', file=sys.stderr)
		print('\tExpression: '+searchXpatch, file=sys.stderr)
	if conflictsZadany==1:
		print('\tConflicts je zadane.', file=sys.stderr)

#spracovanie

string = getInput(inputZadany)
if err==1 :
	sys.exit(2)
	print('chyba vstupu', file=sys.stderr)
#print("qwertyuiop")
#if not re.match(r'\s*(class\s+\w+\s*(:\s*(((protected)|(public)|(private))\s+)?\w+\s*(,\s*(((protected)|(public)|(private))\s+)?\w+\s*)*)?({(\s*(((protected)|(public)|(private))\s*:\s*)?(((?#pointer)(?#typ)(\w+)\s*(?#pointer)(\*\s*)+(?#identifikator)\w+\s*(\s*((=\s*\w+)))?\s*;\s*)|((?#premenna)((un)?signed\s+)?((short|long)\s+)?(?#typ)(\w+)\s+(?#identifikator)\w+\s*(\s*((=\s*\w+)))?\s*;\s*)|((?#using)using\s+\w+\s*::\s*\w+\s*;\s*)|((?#constructor a destructor)(\~)?\w+\s*\(\s*\)\s*\{\s*\}\s*(;)?\s*)|((?#metody)(?#virtual)(virtual\s+)?((un)?signed\s+)?((short|long)\s+)?(?#typ)(\w+)\s+(?#identifikator)(\w+)\s*(\(\s*(void|(((\w+\s*(\*|\&)\s*\w+)|(\w+\s+\w+\s*(=\w)?))(\s*,\s*((\w+\s*(\**|\&)\s*\w+)|(\w+\s+\w+)))*))?\s*\)\s*)(({\s*}(\s*;)?)|;|=0;)\s*)))*})?\s*;\s*)+',string):
#	sys.exit(4)
if string.isspace() or string=="":
	xml="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<model />"
	outPut(xml,outputZadany)
	if err==1 :
		sys.exit(3)
	sys.exit(0)
#string = re.sub(r'\n', ' ', string)

#string = re.sub(r'\t', ' ', string)

#string = re.sub(r' +', ' ', string)

#rozdelenie tried na samostatne ridky
string = re.sub(r'\s*(})?\s*;\s*(class\s*|$)', '\nclass ', string)
#odstranenie posledneho odriadkovania
string = re.sub(r'\nclass\s*$', '', string)

string = re.sub(r'^\s*class\s*', '', string)

#rozdelenie na jednotlive classy
arrayOfClasses=string.split("\nclass ")
list=[]

#print(arrayOfClasses, file=sys.stderr)
#cycle all classes
for (i, item) in enumerate(arrayOfClasses):
	actualModifier="private";
	if debug==1:
		print("spracuvava sa:<<<"+item+">>>", file=sys.stderr)
	if re.match(r'^\s*([a-zA-Z_][0-9a-zA-Z_]*)\s*(:\s*(((private|protected|public)\s+)?[a-zA-Z_][0-9a-zA-Z_]*)\s*(,\s*((private|protected|public)\s+)([a-zA-Z_][0-9a-zA-Z_]*))*)?\s*$',item):
		#treba si ulozit nazov triedy, lebo musi byt neskor definovany
		if debug==1:
			print("debug", file=sys.stderr)
		continue
		
	ddd=item.split("{",1)
	itemClass=ddd[0].split(":",1)
	# if itemClass[0].split(" ",1)[0]!="class":
		# print("chyba class_", file=sys.stderr)
		# sys.exit(4)
	
	className=zmazMedzeruPredAZa(itemClass[0])
	if not isIdentifier(className):
		print(className+" nieje validny identifikator", file=sys.stderr)
		sys.exit(4)
	
	ArrayOfInheritance=[]
	if len(itemClass)>1:
		ArrayOfInheritance=itemClass[1].split(",")
	
	j=0
	for e in ArrayOfInheritance:
		ArrayOfInheritance[j]=zmazMedzeruPredAZa(e)
		
		#rozdelit 
		ArrayOfInheritance[j]=ArrayOfInheritance[j].split(" ")
		lenght = len(ArrayOfInheritance[j])
		#otestovat ci uz dana trieda sa nachadza v zozname
		if testIfUniqueClass(ArrayOfInheritance[j][lenght-1])==0:
			#chyba nedefinovana trieda
			sys.exit(4)
		j+=1
		
	for idx, e in enumerate(ArrayOfInheritance):
		for index in range(0,idx-1):
			if ArrayOfInheritance[idx][len(ArrayOfInheritance[idx])-1]==ArrayOfInheritance[index][len(ArrayOfInheritance[index])-1]:
				print("Dedenie z tried "+ArrayOfInheritance[idx][len(ArrayOfInheritance[idx])-1]+" na indexe "+idx+" a "+ArrayOfInheritance[index][len(ArrayOfInheritance[index])-1]+" na indexe "+index +"sa zhoduju.", file=sys.stderr)
				sys.exit(21)
		
	
	#atributy a metody
	
	PublicSet=0
	PrivateSet=0
	ProtectedSet=0
	
	
	ArrayOfPublicVariables=[]
	ArrayOfPrivateVariables=[]
	ArrayOfProtectedVariables=[]
	
	ArrayOfPublicMethods=[]
	ArrayOfPrivateMethods=[]
	ArrayOfProtectedMethods=[]
	
	using=[]
	
	# pridanie medzery pred a za bodkociarku
	# ddd[1] = re.sub(r' *; *', ' ; ', ddd[1])
	
	
	if debug==1:
		print("ostava spracovat:"+ddd[1], file=sys.stderr)
	setLexicalString(ddd[1])
	state=0
	isFunction=0
	isAtribute=0
	IsStatic="instance"
	IsVirtual="noVirtual"
	IsSigned=0
	IsUnsigned=0
	IsShort=0
	IsLong=0
	type=""
	sumOfAttributes=1
	
	while endOfString==0:
		IsStatic="instance"
		IsVirtual="noVirtual"
		isFunction=0
		tokenA=nextToken()
		if endOfString==1:
			break
		
		if state==0:
			if tokenA=="public" or  tokenA=="private" or  tokenA=="protected":
				# treba zmenit aktualny modifikator
				# if sumOfAttributes==0:
					# sys.exit(21)
					#chyba, prazdne
				if tokenA=="public":
					PublicSet=1
					actualModifier="public";
				
				if tokenA=="private":
					PublicSet=1
					actualModifier="private";
				
				if tokenA=="protected":
					PublicSet=1
					actualModifier="protected";
				
				tokenB=nextToken()
				if tokenB !=":":
					print("chyba syntaxe, za "+tokenA+" sa nenachadza dvojbodka.", file=sys.stderr)
					sys.exit(4)
				
				sumOfAttributes=0
				continue
			if tokenA==";":
				#preskocime prazdny prikaz
				continue
			
			sumOfAttributes+=1
			
			if tokenA=="using":
				tokenA=nextToken()
				Inheritefrom=tokenA
				tokenA=nextToken()
				if tokenA!="::":
					#chyba
					sys.err(4)
				tokenA=nextToken()
				identifikator=tokenA
				if not isIdentifier(identifikator):
					print("chyba syntaxe USING,chybny identifikator: "+identifikator, file=sys.stderr)
					sys.exit(4)
				
				tokenA=nextToken()
				if tokenA !=";":
					print("chyba syntaxe USING, za using sa nenachadza bodkociarka.", file=sys.stderr)
					sys.exit(4)
				using.append([Inheritefrom,identifikator])
				continue
			
			#konstructor a destructor:
			elif tokenA==className:
				#vlozenie konstruktora medzi metody:
				tokenA=nextToken()
				if tokenA!="(":
					#chyba
					print("chyba syntaxe CONSTRUCTOR, chyba ( .", file=sys.stderr)
					sys.exit(4)
				#spracovanie argumentov
				
				
				methodArguments=[]
				
				
				
				tokenA=nextToken()
				#argementy TODO
				
				argTypeList=[]
				
				
				
				if tokenA=="void":
					tokenA=nextToken()
					
				elif tokenA!=")" :
					if tokenA=="signed" or tokenA=="unsigned":
						if tokenA=="signed":
							argTypeList.append("signed")
							argIsSigned=1
						elif tokenA=="unsigned":
							argTypeList.append("unsigned")
							argIsUnsigned=1
						tokenA=nextToken()
					
					if tokenA=="short" or tokenA=="long":
						if tokenA=="short":
							argTypeList.append("short")
							argIsShort=1
						elif tokenA=="long":
							argTypeList.append("long")
							argIsLong=1
						tokenA=nextToken()
					
					#typ
					argtype=tokenA
					
					if not isIdentifier(argtype):
						print("chyba syntaxe,chybny typ: "+argtype, file=sys.stderr)
						sys.exit(4)
					tokenA=nextToken()
					
					if tokenA=="*":
						argtype+=" "
						while tokenA=="*":
							argtype+=tokenA
							tokenA=nextToken()
					elif tokenA=="&":
						argtype+=" "+tokenA
						tokenA=nextToken()
					
					
					argTypeList.append(argtype)
					#test type
					if not testIfValidType(argTypeList):
						sys.exit(4)
					
					
					#identifikator
					identifier=tokenA
					if not isIdentifier(identifier):
						print("chyba syntaxe B,chybny identifikator: "+identifier, file=sys.stderr)
						sys.exit(4)
					tokenA=nextToken()
					
					if tokenA=="=":
						tokenA=nextToken()
						if tokenTYPE=="special":
							#chyba
							if debug==1:
								print("chyba syntaxe, priradovanie do premennej specialny znak "+tokenA, file=sys.stderr)
							sys.exit(4)
						tokenA=nextToken()
					#vlozenie do pola argumentov
					methodArguments.append([argTypeList,identifier])
					
					while tokenA==",":
						
						tokenA=nextToken()
						argTypeList=[] #vyprazdnime list
						if tokenA=="signed" or tokenA=="unsigned":
							if tokenA=="signed":
								argIsSigned=1
								argTypeList.append("signed")
							elif tokenA=="unsigned":
								IsUnsigned=1
								argTypeList.append("unsigned")
							tokenA=nextToken()
						
						if tokenA=="short" or tokenA=="long":
							if tokenA=="short":
								argTypeList.append("short")
								argIsShort=1
							elif tokenA=="long":
								argTypeList.append("long")
								argIsLong=1
							tokenA=nextToken()
						
						#typ
						argtype=tokenA
						if not isIdentifier(argtype):
							print("chyba syntaxe,chybny typ: "+argtype, file=sys.stderr)
							sys.exit(4)
						tokenA=nextToken()
						
						if tokenA=="*":
							argtype+=" "
							while tokenA=="*":
								argtype+=tokenA
								tokenA=nextToken()
						elif tokenA=="&":
							argtype+=" "+tokenA
							tokenA=nextToken()
						
						
						
						
						argTypeList.append(argtype)
						if not testIfValidType(argTypeList):
							sys.exit(4)
						
						
						#identifikator
						identifier=tokenA
						if not isIdentifier(identifier):
							print("chyba syntaxe C,chybny identifikator: "+identifier, file=sys.stderr)
							sys.exit(4)
						tokenA=nextToken()
						
						if tokenA=="=":
							tokenA=nextToken()
							if tokenTYPE=="special":
								#chyba
								
								if debug==1:
									print("chyba syntaxe, priradovanie do premennej specialny znak "+tokenA, file=sys.stderr)
								sys.exit(4)
							tokenA=nextToken()
						
						#ulozenie
						
						
						
						methodArguments.append([argTypeList,identifier])
						
						
				
				
				
				
				
				
				#methodArguments.append()
				
				
				if tokenA!=")":
					#chyba
					print("chyba syntaxe CONSTRUCTOR, chyba ) .", file=sys.stderr)
					sys.exit(4)
				
				tokenA=nextToken()
				if tokenA=="{":
					#definicia
					tokenA=nextToken()
					if tokenA!="}":
						#chyba - neukoncena zatvorka
						print("chyba syntaxe CONSTRUCTOR, chyba}.", file=sys.stderr)
						sys.exit(4)
					#vkladame definiciu
					
					
					
				elif not tokenA==";":
					#chyba
					print("chyba syntaxe CONSTRUCTOR, chyba ;.", file=sys.stderr)
					sys.exit(4)
				#vkladame deklaraciu
				
				
				saveMethod(className,[className],"instance",methodArguments,"noVirtual")
				continue
			
			
			elif tokenA=="~":
				tokenA=nextToken()
				if tokenA!=className:
					#chyba
					print("chyba syntaxe DESTRUCTOR, identifikator sa nerovna k triede.", file=sys.stderr)
					sys.exit(4)
				tokenA=nextToken()
				if tokenA!="(":
					print("chyba syntaxe DESTRUCTOR, chyba (.", file=sys.stderr)
					#chyba
					sys.exit(4)
				
				tokenA=nextToken()
				#spracovanie argumentov - v destruktore niesu
				if tokenA!=")":
					#chybaDESTRUCTOR
					sys.exit(4)
				
				tokenA=nextToken()
				if tokenA=="{":
					#definicia
					tokenA=nextToken()
					if tokenA!="}":
						print("chyba syntaxe DESTRUCTOR, chyba}.", file=sys.stderr)
						#chyba - neukoncena zatvorka
						sys.exit(4)
					#vkladame definiciu
					
					#saveMethod(className,"~"+className,"instance",methodArguments,0)
					
					
				elif  not tokenA==";":
					#chyba
					print("chyba syntaxe DESTRUCTOR, chyba ;.", file=sys.stderr)
					sys.exit(4)
				#vkladame deklaraciu
				
				saveMethod("~"+className,["void"],"instance",methodArguments,"noVirtual")
				#vlozenie destruktora medzi metody
				continue
			
			if tokenA=="static":
				IsStatic="static"
				tokenA=nextToken()
			
			if tokenA=="virtual":
				if IsStatic=="static":
					print("static virtual.", file=sys.stderr)
					sys.exit(4)
				IsVirtual="virtual"
				isFunction=1
				tokenA=nextToken()
			TypeList=[]
			if tokenA=="signed" or tokenA=="unsigned":
				if tokenA=="signed":
					TypeList.append("signed")
					IsSigned=1
				elif tokenA=="unsigned":
					TypeList.append("unsigned")
					IsUnsigned=1
				tokenA=nextToken()
			
			if tokenA=="short" or tokenA=="long":
				if tokenA=="short":
					TypeList.append("short")
					IsShort=1
				elif tokenA=="long":
					TypeList.append("long")
					IsLong=1
				tokenA=nextToken()
			
			#typ
			type=tokenA
			if not isIdentifier(type):
				print("chyba syntaxe,chybny typ: "+type, file=sys.stderr)
				sys.exit(4)
			tokenA=nextToken()
			
			if tokenA=="*":
				type+=" "
				while tokenA=="*":
					type+=tokenA
					tokenA=nextToken()
			
			TypeList.append(type)
			if not testIfValidType(TypeList):
				sys.exit(4)
			
			#identifikator
			identifier=tokenA
			if not isIdentifier(identifier):
				print("chyba syntaxe A,chybny identifikator: "+identifier, file=sys.stderr)
				sys.exit(4)
			tokenA=nextToken()
			
			
			
			methodArguments=[]
			argTypeList=[]
			
			
			
			if tokenA==";":
				if isFunction==1:
					#chyba bolo zadane virtual pre atribut
					print("chyba syntaxe,virtualny atribut: " + identifier, file=sys.stderr)
					sys.exit(4)
				
				
				saveVariable(identifier,TypeList,IsStatic)
				
				#uloz premennu s danymi parametrami
			elif tokenA=="=":
				tokenA=nextToken()
				if tokenTYPE=="special":
					#chyba
					print("chyba syntaxe, za = nemoze nasledovat specialny znak "+tokenA, file=sys.stderr)
					sys.exit(4)
				
				saveVariable(identifier,TypeList,IsStatic)
			elif tokenA=="(":
				tokenA=nextToken()
				#argementy
				if tokenA=="void":
					tokenA=nextToken()
					
				elif tokenA!=")" :
					if debug==1:
						print(">"+tokenA+"<", file=sys.stderr)
					
					
					if tokenA=="signed" or tokenA=="unsigned":
						if tokenA=="signed":
							argTypeList.append("signed")
							argIsSigned=1
						elif tokenA=="unsigned":
							argTypeList.append("unsigned")
							argIsUnsigned=1
						tokenA=nextToken()
					
					if tokenA=="short" or tokenA=="long":
						if tokenA=="short":
							argTypeList.append("short")
							argIsShort=1
						elif tokenA=="long":
							argTypeList.append("long")
							argIsLong=1
						tokenA=nextToken()
					
					#typ
					argtype=tokenA
					if not isIdentifier(argtype):
						print("chyba syntaxe,chybny typ: "+argtype, file=sys.stderr)
						sys.exit(4)
					tokenA=nextToken()
					
					if tokenA=="*":
						argtype+=" "
						while tokenA=="*":
							argtype+=tokenA
							tokenA=nextToken()
					elif tokenA=="&":
						argtype+=" "+tokenA
						tokenA=nextToken()
					
					
					argTypeList.append(argtype)
					if not testIfValidType(argTypeList):
							sys.exit(4)
						
					#identifikator
					argIdentifier=tokenA
					if not isIdentifier(argIdentifier):
						print("chyba syntaxe B,chybny identifikator: "+argIdentifier, file=sys.stderr)
						sys.exit(4)
					tokenA=nextToken()
					
					if tokenA=="=":
						tokenA=nextToken()
						if tokenTYPE=="special":
							#chyba
							print("chyba syntaxe, priradovanie do premennej specialny znak "+tokenA, file=sys.stderr)
						tokenA=nextToken()
					#vlozenie do pola argumentov
					
					methodArguments.append([argTypeList,argIdentifier])
					while tokenA==",":
						tokenA=nextToken()
						argTypeList=[]
						if tokenA=="signed" or tokenA=="unsigned":
							if tokenA=="signed":
								argTypeList.append("signed")
								IsSigned=1
							elif tokenA=="unsigned":
								argTypeList.append("unsigned")
								IsUnsigned=1
							tokenA=nextToken()
						
						if tokenA=="short" or tokenA=="long":
							if tokenA=="short":
								argTypeList.append("short")
								IsShort=1
							elif tokenA=="long":
								argTypeList.append("long")
								IsLong=1
							tokenA=nextToken()
						
						#typ
						argtype=tokenA
						if not isIdentifier(argtype):
							print("chyba syntaxe,chybny typ: "+argtype, file=sys.stderr)
							sys.exit(4)
						tokenA=nextToken()
						
						if tokenA=="*":
							argtype+=" "
							while tokenA=="*":
								argtype+=tokenA
								tokenA=nextToken()
						elif tokenA=="&":
							argtype+=" "+tokenA
							tokenA=nextToken()
						
						argTypeList.append(argtype)
						if not testIfValidType(argTypeList):
							sys.exit(4)
						
						#identifikator
						argIdentifier=tokenA
						if not isIdentifier(argIdentifier):
							print("chyba syntaxe C,chybny identifikator: "+argIdentifier, file=sys.stderr)
							sys.exit(4)
						tokenA=nextToken()
						
						if tokenA=="=":
							tokenA=nextToken()
							if tokenTYPE=="special":
								#chyba
								print("chyba syntaxe, priradovanie do premennej specialny znak "+tokenA, file=sys.stderr)
							tokenA=nextToken()
						
						methodArguments.append([argTypeList,argIdentifier])
						
						
				
				
				
				
				
				
				
				
				if tokenA!=")":
					#chyba
					print("chyba syntaxe, chyba ).", file=sys.stderr)
					sys.exit(4)
				tokenA=nextToken()
				if tokenA=="{":
					tokenA=nextToken()
					if tokenA!="}":
						#chyba
						print("chyba syntaxe, chyba }.", file=sys.stderr)
						sys.exit(4)
					#ulozenie definicie
					#saveMethod(identifier,TypeList,IsStatic,methodArguments,IsVirtual)
					
				elif tokenA=="=":
					tokenA=nextToken()
					if IsVirtual!="virtual" or tokenA!="0":
						print("chyba syntaxe, ak je za funkciou = musi byt virtualna a nasledovat0.", file=sys.stderr)
						sys.exit(4)
					IsVirtual="pure virtual"
				elif tokenA!=";":
					#chyba
					print("chyba syntaxe, chyba ;.", file=sys.stderr)
					sys.exit(4)
				#ulozenie deklaracie
				saveMethod(identifier,TypeList,IsStatic,methodArguments,IsVirtual)
				
			else:
				#chyba nedefinovana sekvencia tokenov
				print("chyba syntaxe,nedefinovana sekvencia tokenov.", file=sys.stderr)
				sys.exit(4)
			
			#treba zistit ci sa jedna o metodu alebo atribut
			
			
			
			
			
	# if sumOfAttributes==0:
		# print("Prazdny modifikator!", file=sys.stderr)
		# sys.exit(21)
	
	
	
	
	
	#print("modifikator "+ str(sumOfAttributes), file=sys.stderr)
	
	
	
	
	
	
	# modifiersFound=re.findall(r'( public: )|( private: )|( protected: )', ddd[1])
	# pomocnaPremenna=re.split(r'( public: )|( private: )|( protected: )', ddd[1],1)
	# argumentsWithout=pomocnaPremenna[0]
	
	# print(modifiersFound, file=sys.stderr)
	# print("qwerty:"+argumentsWithout, file=sys.stderr)
	
	# if len(modifiersFound)>0:
		# other=pomocnaPremenna[1]
	
	
	
		# print("other:"+other, file=sys.stderr)
	# # pointre odelime
	# ddd[1] = re.sub(r' *\* *', '* ', ddd[1])
	# words = ddd[1].split()
	# try:
		# for x in range(0, len(words)):
		
			# if words[x]=="public:" or  words[x]=="private:" or  words[x]=="protected:":
				# # treba zmenit aktualny modifikator
				# if words[x]=="public:":
					# PublicSet=1
					# actualModifier="public";
				
				# if words[x]=="private:":
					# PublicSet=1
					# actualModifier="private";
				
				# if words[x]=="protected:":
					# PublicSet=1
					# actualModifier="protected";
			
			# else:
				# # test ci je 
				# scope="instance"
				# signed=1
				# if words[x]=="virtual":
					# scope="virtual"
					# x=x+1
				
				# if re.match(r'(signed)|(unsigned)',words[x]):
					# if words[x]=="unsigned":
						# signed=0
				
				
				# # todo dokoncit argumenty a metody
				# # todo konecne zacat naspracovani!
				
				
				
				
				
			
			
	# except IndexError:
		# print("chyba indexu", file=sys.stderr)
		# sys.exit(4)
	
	
	list.append({
	'Name': className,
	'Using': using,
	'InheritanceFrom': ArrayOfInheritance,
	'VariablePublic': ArrayOfPublicVariables,
	'VariablePrivate': ArrayOfPrivateVariables,
	'VariableProtected': ArrayOfProtectedVariables,
	'MethodsPublic': ArrayOfPublicMethods,
	'MethodsPrivate': ArrayOfPrivateMethods,
	'MethodsProtected': ArrayOfProtectedMethods
	})

	#dedenie
list1=[]
for classitem in list:
	if debug==1:
		print(classitem["Name"], file=sys.stderr)
	className=classitem["Name"]
	using=classitem["Using"]
	ArrayOfInheritance=classitem["InheritanceFrom"]
	ArrayOfPublicVariables=[]
	ArrayOfPrivateVariables=[]
	ArrayOfProtectedVariables=[]
	ArrayOfPublicMethods=[]
	ArrayOfPrivateMethods=[]
	ArrayOfProtectedMethods=[]
	
	for inheritance in classitem["InheritanceFrom"]:
		if len(inheritance)==1:
			privacy="private"
			NameOfClassToInheritance=inheritance[0]
		else:
			privacy=inheritance[0]
			NameOfClassToInheritance=inheritance[1]
		
		# najdenie spravnej triedy v liste
		for inheritanceClass in list1:
			if inheritanceClass["Name"]==NameOfClassToInheritance:
				
				
				# public
				actualModifier=privacy
				
				for variable in inheritanceClass["VariablePublic"]:
					saveVariable(variable["Name"],[variable["Type"]],variable["Scope"],inheritanceClass["Name"] if variable["From"]=="0" else variable["From"],"yes")
					
					
				
				for method in inheritanceClass["MethodsPublic"]:
					saveMethod(method["Name"],[method["Type"]],method["Scope"],method["Arguments"],method["Virtual"],inheritanceClass["Name"] if method["From"]=="0" else method["From"],"yes")
				
				
				
				
				
				# protected
				if privacy=="private":
					actualModifier="private"
				else:
					actualModifier="protected"
				
				for variable in inheritanceClass["VariableProtected"]:
					saveVariable(variable["Name"],[variable["Type"]],variable["Scope"],inheritanceClass["Name"] if variable["From"]=="0" else variable["From"],"yes")
				actualModifier=privacy
				
				for method in inheritanceClass["MethodsProtected"]:
					saveMethod(method["Name"],[method["Type"]],method["Scope"],method["Arguments"],method["Virtual"],inheritanceClass["Name"] if method["From"]=="0" else method["From"],"yes")
				
				
				
				
				
				
				
				# private
				actualModifier="private"
				
				for variable in inheritanceClass["VariablePrivate"]:
					saveVariable(variable["Name"],[variable["Type"]],variable["Scope"],inheritanceClass["Name"] if variable["From"]=="0" else variable["From"],"no")
				
				for method in inheritanceClass["MethodsPrivate"]:
					saveMethod(method["Name"],[method["Type"]],method["Scope"],method["Arguments"],method["Virtual"],inheritanceClass["Name"] if method["From"]=="0" else method["From"],"yes")
				
				
				
				
				
				
	
	
	
	
	
	
	#nasleduje pridavanie vlastnych a kontrola ci nenastava kolizia
	
	
	
	#kontrolovat sa bude pocas pridavania a ak sa najde zhoda,
	#tak sa odstrania tie ktore su dedene a nahradia sa tym ktore je z aktualnej triedy
	
	
	
	
	#najdenie triedy
	for classItemOrc in list:
		#hladanie
		
		
		#vlozenie
		if classItemOrc["Name"]==classitem["Name"]:
			for murloc in classItemOrc["VariablePublic"]:
				vyhladajKonflikt(murloc["Name"],murloc["Type"],True)
				ArrayOfPublicVariables.append(murloc)
				
			for murloc in classItemOrc["VariablePrivate"]:
				vyhladajKonflikt(murloc["Name"],murloc["Type"],True)
				ArrayOfPrivateVariables.append(murloc)
				
			for murloc in classItemOrc["VariableProtected"]:
				vyhladajKonflikt(murloc["Name"],murloc["Type"],True)
				ArrayOfProtectedVariables.append(murloc)
			
			
			for murloc in classItemOrc["MethodsPublic"]:
				vyhladajKonflikt(murloc["Name"],murloc["Type"])
				ArrayOfPublicMethods.append(murloc)
			for murloc in classItemOrc["MethodsPrivate"]:
				vyhladajKonflikt(murloc["Name"],murloc["Type"])
				ArrayOfPrivateMethods.append(murloc)
			for murloc in classItemOrc["MethodsProtected"]:
				vyhladajKonflikt(murloc["Name"],murloc["Type"])
				ArrayOfProtectedMethods.append(murloc)
	
	#tu treba zasa testovat aby sme vedeli ci este nenastava niekde kolizia 
	#a ak ano vyskusame ci nieje v using napisane ze z ktorej dedit 
	#a ak ani tam nieje tak vyhodime chybu 21
	
	#ostava este using TODO
	
	for index, item in enumerate(ArrayOfPublicVariables):
		tryToFind(item,index,ArrayOfPublicVariables)
	for index, item in enumerate(ArrayOfPrivateVariables):
		tryToFind(item,index,ArrayOfPrivateVariables)
	for index, item in enumerate(ArrayOfProtectedVariables):
		tryToFind(item,index,ArrayOfProtectedVariables)
	for index, item in enumerate(ArrayOfPublicMethods):
		tryToFind(item,index,ArrayOfPublicMethods)
	for index, item in enumerate(ArrayOfPrivateMethods):
		tryToFind(item,index,ArrayOfPrivateMethods)
	for index, item in enumerate(ArrayOfProtectedMethods):
		tryToFind(item,index,ArrayOfProtectedMethods)
	
	#treba zistit ci je abstract alebo concrete !!!!! TODO
	
	kind ="concrete"
	for item in ArrayOfPublicMethods:
		if item["Virtual"]=="pure virtual":
			kind ="abstract"
	for item in ArrayOfPrivateMethods:
		if item["Virtual"]=="pure virtual":
			kind ="abstract"
	for item in ArrayOfProtectedMethods:
		if item["Virtual"]=="pure virtual":
			kind ="abstract"
	
			#pure virtual sa tlacia tiez
			item["printable"]="yes"
	
	list1.append({
	'Name': className,
	'Kind': kind,
	'Using': using,
	'InheritanceFrom': ArrayOfInheritance,
	'VariablePublic': ArrayOfPublicVariables,
	'VariablePrivate': ArrayOfPrivateVariables,
	'VariableProtected': ArrayOfProtectedVariables,
	'MethodsPublic': ArrayOfPublicMethods,
	'MethodsPrivate': ArrayOfPrivateMethods,
	'MethodsProtected': ArrayOfProtectedMethods
	})


list = list1

if debug==1:
	print("------------------------------------------------", file=sys.stderr)
	for e in list:
		print("Class name:"+ e['Name'], file=sys.stderr)
		for key, value in e.items():
			if value!= e['Name'] and value!=[]:
				print("\t"+key +" : ", file=sys.stderr)
				for kkk in value:
					print("\t\t"+str(kkk), file=sys.stderr)
		
	print("------------------------------------------------", file=sys.stderr)

if detailsZadany==0:
	arrayOfArrayOfClasses=[]
	
	xml=header
	if detailsZadany==0:
		xml+="<model>\n"
	for e in list:
		arrayOfClasses=[]
		
		
		if e['InheritanceFrom']==[]:
			xml+=("<class name=\""+ e["Name"] +"\" kind=\""+e['Kind']+"\">\n</class>\n")
		else:
			for i in e['InheritanceFrom']:
				Founds= re.finditer("<class name=\""+i[len(i)-1] +"\" kind=\"(abstract|concrete)\">\n",xml )
				
				asdfghfdsa=[]
				for Found in Founds:
					asdfghfdsa.append(Found.end())
				
				
				for Found in reversed(asdfghfdsa):
					positionEnd=Found
					
					
					xml= xml[:positionEnd] + "<class name=\""+ e["Name"] +"\" kind=\""+e['Kind']+"\">\n</class>\n" + xml[positionEnd:]
					
				
				
				
				
				#arrayOfClasses.append(i[len(i)-1])
		
		#arrayOfClasses.append(e['Name'])
		
		
		
		
		
		
		
		
		
		
		
		
		
		#stre=recursivneVyhladavanieCesty(str)
		
		#print(arrayOfClasses,file=sys.stderr)
		#arrayOfArrayOfClasses.append(arrayOfClasses)
		

	#odstranenie 
	xml=re.sub(r'(<[^/]([^\n])*)>\n</[^\n]*>',r'\1 />',xml)
		
		
		

	# for ClassItems in arrayOfArrayOfClasses:
		# xml+="<class name=\""+Classname +"\" kind=\""abstract"\">"
		
		

	if detailsZadany==0:
		xml+="</model>\n"

	model = ET.fromstring(xml)

	#odriadkovavanie
	# if prettyZadany==1:
		# iterator = 4
	# elif prettyZadany==2:
		# iterator = prettyXmlK
	# else:
		# iterator=4
	# pocetMedzier=-iterator
	# xmla=xml.split("\n")
	# xml=""
	# qwdasfdgfhgjhkugfbvdxhngjgbfdrghjnmhgfdrghjkljhgfdsxcfgv=0
	# for riadok in xmla:
		# medzera=""
		
		# if not re.match(r'(<\?xml version=\"1.0\" encoding=\"UTF-8\"\?>)',riadok):
			# print("$"+str(pocetMedzier)+" "+riadok,file=sys.stderr)
			
			# if not re.match(r'^\s*<[^/].*/>',riadok):
				# if re.match(r'^\s*<[^/].*>',riadok):
					# if qwdasfdgfhgjhkugfbvdxhngjgbfdrghjnmhgfdrghjkljhgfdsxcfgv==1:
						# qwdasfdgfhgjhkugfbvdxhngjgbfdrghjnmhgfdrghjkljhgfdsxcfgv=0
						# print("nothing",file=sys.stderr)
					# else:
						# pocetMedzier+=iterator
						# if debug==1:
							# print("zvacsenie",file=sys.stderr)
				# elif re.match(r'^\s*</.*',riadok):
					# if debug==1:
						# print("zmensenie",file=sys.stderr)
					# pocetMedzier-=iterator
					# qwdasfdgfhgjhkugfbvdxhngjgbfdrghjnmhgfdrghjkljhgfdsxcfgv=1
			
			# for i in range(0,pocetMedzier):
				# medzera+=" "
				
				
		# xml+=medzera+riadok+"\n"
else:
	model = ET.Element("model")
	for classItem in list:
		
		
		publicSetV=0
		privateSetV=0
		protectedSetV=0
		publicSetM=0
		privateSetM=0
		protectedSetM=0
		
		class_ = ET.SubElement(model, "class", name=classItem["Name"], kind=classItem["Kind"])
		if classItem["InheritanceFrom"]!=[]:
			inheritance = ET.SubElement(class_, "inheritance")
			for item in classItem["InheritanceFrom"]:
				if len(item)==1:
					ET.SubElement(inheritance, "from", name=item[0],privacy="private")
				else:
					ET.SubElement(inheritance, "from", name=item[1],privacy=item[0])
		
		if classItem["VariablePublic"]!=[] or classItem["MethodsPublic"]!=[]:
			if publicSetV==0 and publicSetM==0:
				public = ET.SubElement(class_, "public")
			
			if classItem["VariablePublic"]!=[]:
				if publicSetV==0:
					atributespublic= ET.SubElement(public, "attributes")
					publicSetV=1
				for item in  classItem["VariablePublic"]:
					if item["printable"]=="yes":
						var = ET.SubElement(atributespublic, "attribute",name=item["Name"], type=item["Type"],scope=item["Scope"])
						
						if item["From"]!="0":
							ET.SubElement(var,"from", name=item["From"])
		
			
			
			
			
			
			if classItem["MethodsPublic"]!=[]:
				if publicSetM==0:
					methodspublic= ET.SubElement(public, "methods")
					publicSetM=1
				for item in  classItem["MethodsPublic"]:
					if item["printable"]=="yes":
						if item["Virtual"]=="pure virtual":
							metoda=ET.SubElement(methodspublic, "method",name=item["Name"], type=item["Type"],scope=item["Scope"])
							ET.SubElement(metoda, "virtual",pure="yes")
						elif item["Virtual"]=="virtual":
							metoda=ET.SubElement(methodspublic, "method",name=item["Name"], type=item["Type"],scope=item["Scope"])
							ET.SubElement(metoda, "virtual",pure="no")
						else:
							metoda=ET.SubElement(methodspublic, "method",name=item["Name"], type=item["Type"],scope=item["Scope"])
						
						if item["From"]!="0":
							ET.SubElement(metoda,"from", name=item["From"])
						
						arguments=ET.SubElement(metoda, "arguments")
						for argument in item["Arguments"]:
							ET.SubElement(arguments, "argument",name=argument[1], type=spojTypy(argument[0]))

					
					
					
		if classItem["VariablePrivate"]!=[] or classItem["MethodsPrivate"]!=[]:
			if privateSetV==0 and privateSetM==0:
				private = ET.SubElement(class_, "private")
			
			if classItem["VariablePrivate"]!=[]:
				if privateSetV==0:
					atributesprivate= ET.SubElement(private, "attributes")
					privateSetV=1
				for item in  classItem["VariablePrivate"]:
					if item["printable"]=="yes":
					# <attribute name="b" type="int *" scope="instance" />
					
						var=ET.SubElement(atributesprivate, "attribute",name=item["Name"], type=item["Type"],scope=item["Scope"])
						if item["From"]!="0":
							ET.SubElement(var,"from", name=item["From"])

			
			
			
			
			
			
			if classItem["MethodsPrivate"]!=[]:
				if privateSetM==0:
					methodsprivate= ET.SubElement(private, "methods")
					privateSetM=1
				for item in  classItem["MethodsPrivate"]:
					if item["printable"]=="yes":
						if item["Virtual"]=="pure virtual":
							metoda=ET.SubElement(methodsprivate, "method",name=item["Name"], type=item["Type"],scope=item["Scope"])
							ET.SubElement(metoda, "virtual",pure="yes")
						elif item["Virtual"]=="virtual":
							metoda=ET.SubElement(methodsprivate, "method",name=item["Name"], type=item["Type"],scope=item["Scope"])
							ET.SubElement(metoda, "virtual",pure="no")
						else:
							metoda=ET.SubElement(methodsprivate, "method",name=item["Name"], type=item["Type"],scope=item["Scope"])
						
						if item["From"]!="0":
							ET.SubElement(metoda,"from", name=item["From"])
						arguments=ET.SubElement(metoda, "arguments")
						for argument in item["Arguments"]:
							ET.SubElement(arguments, "argument",name=argument[1], type=spojTypy(argument[0]))
					

					
					
					
		if classItem["VariableProtected"]!=[] or classItem["MethodsProtected"]!=[]:
			if protectedSetV==0 and protectedSetM==0:
				protected = ET.SubElement(class_, "protected")
			
			if classItem["VariableProtected"]!=[]:
				if protectedSetV==0:
					atributesprotected= ET.SubElement(protected, "attributes")
					protectedSetV==1
				for item in  classItem["VariableProtected"]:
					if item["printable"]=="yes":
						var=ET.SubElement(atributesprotected, "attribute",name=item["Name"], type=item["Type"],scope=item["Scope"])
						if item["From"]!="0":
							ET.SubElement(var,"from", name=item["From"])
				
			
			
			
			
			
			if classItem["MethodsProtected"]!=[]:
				if protectedSetM==0:
					methodsprotected= ET.SubElement(protected, "methods")
					protectedSetM=1
				for item in  classItem["MethodsProtected"]:
					if item["printable"]=="yes":
						if item["Virtual"]=="pure virtual":
							metoda=ET.SubElement(methodsprotected, "method",name=item["Name"], type=item["Type"],scope=item["Scope"])
							ET.SubElement(metoda, "virtual",pure="yes")
						elif item["Virtual"]=="virtual":
							metoda=ET.SubElement(methodsprotected, "method",name=item["Name"], type=item["Type"],scope=item["Scope"])
							ET.SubElement(metoda, "virtual",pure="no")
						else:
							metoda=ET.SubElement(methodsprotected, "method",name=item["Name"], type=item["Type"],scope=item["Scope"])
						
						if item["From"]!="0":
							ET.SubElement(metoda,"from", name=item["From"])
						
						arguments=ET.SubElement(metoda, "arguments")
						for argument in item["Arguments"]:
							ET.SubElement(arguments, "argument",name=argument[1], type=spojTypy(argument[0]))
				
					

	detailsclassis=0
	if detailsZadany==1:
		#vyhladame spravnu triedu v xml a ulozime si 
		for clsss in model:
			if clsss.attrib['name'] == detailsClass:
				detailsclassis=1
				model=clsss
				break
		if detailsclassis==0:
			xml=header
			outPut(xml,outputZadany)
			if err==1 :
				sys.exit(3)
			sys.exit(0)
	


	#ET.SubElement(atributesprotected, "field1", name="F",type="int",scope="instance")
	#m=ET.SubElement(methodsprotected, "method", name="asdfasd").text = "some vlaue2"
	






if prettyZadany==2:
	indent(model,prettyXmlK)
else:
	prettyXmlK=4
	indent(model,4)

xml=header+ET.tostring(model, 'utf-8', method="xml").decode("utf-8") 
	#tree.write(sys.stdout)
	#xml=ET.tostring(model).decode(encoding)
if searchZadany==1:
	
	tree=etree.fromstring(xml.encode('utf-8'),parser=etree.XMLParser(encoding='utf-8'))
	#tree=ET.ElementTree(model)
	#tree= tree.getroot()
	arrayFoundXpath=tree.xpath(searchXpatch)
	
	xml=header+"<result>\n"
	for  sdfg in arrayFoundXpath:
		xml+=prettyXmlK*" "+sdfg+"\n"
	xml+="</result>\n\n"

outPut(xml,outputZadany)
if err==1 :
	sys.exit(3)







