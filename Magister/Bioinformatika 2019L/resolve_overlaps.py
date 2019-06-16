#!/usr/bin/env python3

import sys
#kedze sa mozu lisit na seqid, type a strand, musime si tieto typy zaznamenat
class _SeqTypeClass: #data structure for sequence regions types
	def __init__(self, seqid, type,strand):
		self.seqid = seqid
		self.type = type
		self.strand = strand

seqTypes_list = [];# zoznam typov


class _Sequence: #data structure for sequence regions
	def __init__(self, secFrom, secTo,value,source):
		self.secFrom = secFrom
		self.secTo = secTo
		self.value = value
		self.source = source
		self.parent = -1
		self.seqValue = 0


def SortSeqFuncSecFrom(seq):
	return seq.secFrom


#postupne prejde sekvencie a zisti si hodnotu pre najviac 
def FindNonOverlapsSeq(list,actualSeqType):
	
	#postupne sa prechadzaju sekvencie a pre kazdu sa skontroluje,
	#ci existuje niektora sekvencia z seqLastSeq, s ktorou nekoliduje
	index=1
	
	#pole poslednych, vzajomne kolidujucich prvkov (iba pointer na ne)
	listIndexLastSeq = [0];#index of last sequencies
	
	#prvy zaznam nieje s cim testovat, mozeme si rovno zaznamenat
	iterList = iter(list)
	FirstSeq = next(iterList)
	FirstSeq.seqValue = FirstSeq.value
	
	#zaciname od druheho
	for seq in iterList:
		
		#zoznam najdenych sekvencii, ktore sa s danou sekvencou neprekryvaju
		foundIndexes = []
		maxFrom = 0
		
		for IndexSeqLast in listIndexLastSeq:
			#pri kazdom rade sekvencii, je potrebne najst, od ktorej sekvencie, od konca, sa dana sekvencia nekoliduje
			#kontrolovat je potrebne len, pokial hodnota konca sekvencie nie je  mensia, ako zaciatok najvyssej najdenej nekolidujucej sekvencie
			
			#index ktory sa prave testuje
			testIndex = IndexSeqLast
			
			foundN=False
			#postupne prechadzame po rodicoch, az pokym nieje koniec mensi ako maxFrom
			while True:
				
				#ak sme sa dostali az na zaciatok, alebo je koniec mensi ako maxFrom, 
				#hladanie ukoncime a prehlasime hladanie v danom zozname sekvencii za neuspesene
				if testIndex == -1 or list[testIndex].secTo < maxFrom :
					break
				#ak nekoliduje, zaznamename si ho
				if list[testIndex].secTo < seq.secFrom:
					if list[testIndex].secFrom > maxFrom:
						maxFrom = list[testIndex].secFrom
					foundN=True
					break
				
				testIndex = list[testIndex].parent
			if foundN:
				foundIndexes.append(testIndex)
		
		if len(foundIndexes)>0:
			#Najdenie najlepsieho rodica, s najvyssou hodnotou
			maxValue_index=foundIndexes[0];
			for f_index in foundIndexes:
				#zapise sa, s ktorou ma dana sekvencia najlepsiu hodnotu 
				if list[f_index].seqValue > list[maxValue_index].seqValue:
					maxValue_index=f_index
			
			seq.seqValue = list[maxValue_index].seqValue + seq.value
			seq.parent = maxValue_index
		else:
			seq.seqValue = seq.value
		
		#nasledne je potrebne odstranit sekvencie, ktore nekoliduju s aktualnou sekvenciou
		listIndexLastSeq[:] = [Indexseq for Indexseq in listIndexLastSeq if not (list[Indexseq].secTo < seq.secFrom)]
		
		#a pridat aktualnu sekvenciu
		listIndexLastSeq.append(index)
		
		index = index +1
	
	
	#po spracovani a ohodnoteni vieme, podla hodnoty lastListSeq sekvencii, ktora ma maximalnu hodnotu
	#tuto sekvenciu zoberieme a postupne spatne prechadzame a vypisujeme
	
	#najdenie najvacsej hodnoty
	maxValueIndex = 0
	maxValue=0;
	for IndexSeqLast in listIndexLastSeq:
		if list[IndexSeqLast].seqValue > maxValue:
			maxValue = list[IndexSeqLast].seqValue 
			maxValueIndex = IndexSeqLast
			#print list[IndexSeqLast].seqValue
	
	actualIndex = maxValueIndex
	while True:
		#print list[actualIndex].line,
		print (actualSeqType.seqid,end='')
		print ("\t",end='')
		print (list[actualIndex].source,end='')
		print ("\t",end='')
		print (actualSeqType.type,end='')
		print ("\t",end='')
		print (list[actualIndex].secFrom,end='')
		print ("\t",end='')
		print (list[actualIndex].secTo,end='')
		print ("\t",end='')
		print (list[actualIndex].value,end='')
		print ("\t",end='')
		print (actualSeqType.strand,end='')
		print ("\t",end='')
		print ('.')
		
		if list[actualIndex].parent == -1:
			break
		actualIndex=list[actualIndex].parent



file = open(sys.argv[1])
splitedArray = []
#prechadzame sekvencie a zaznamename si, ake typy sekvencii mame na vstupe.
for line in file:
	if line[0] == '#': #komentar v subore
		print (line,end='')
		continue
	
	splitedArray[:] = line.split("\t")
	#parse seq line
	#ak sa objevi sekvencia s rozdielnimi hodnotami seqid, type alebo strand
	found = False
	for seqT in seqTypes_list:
		if (seqT.seqid == splitedArray[0] and seqT.type == splitedArray[2] and seqT.strand == splitedArray[6]):
			found = True
			break;
	if not found:
		seqTypes_list.append(_SeqTypeClass(splitedArray[0],splitedArray[2],splitedArray[6]));

#ziskali sa vsetky typy, zacneme postupne spracovavat vsetky typy

while len(seqTypes_list) > 0:#pokial nie je seqTypesList prazdny 
	actualSeqType = seqTypes_list.pop(0) #postupne vyberieme typ sekvencii a resetujeme subor
	file.seek(0);
	
	#zoznam vsetkych sekvencii ktore su rovnakeho typu
	seqList = [];
	
	for line in file:#postupne nacitavanie riadkov a ich parsovanie, a ak sa rovna aktualnemu, ulozime si ho
		if line[0] == '#': #komentar v subore
			continue
		splitedArray = line.split("\t")
		if splitedArray[0] == actualSeqType.seqid and splitedArray[2] == actualSeqType.type and splitedArray[6] == actualSeqType.strand:#seqid, type,strand
			seqList.append(_Sequence(int(splitedArray[3]),int(splitedArray[4]),float(splitedArray[5]),splitedArray[1]))#from,to,value
	
	# po tom, ako dojdeme na koniec suboru, zoradime vsetky sekvencie, podla pociatocnej hodnoty
	if not (len(sys.argv) > 2 and sys.argv[2] == "-s"):
		seqList.sort(key=SortSeqFuncSecFrom);
	
	FindNonOverlapsSeq(seqList,actualSeqType)
	