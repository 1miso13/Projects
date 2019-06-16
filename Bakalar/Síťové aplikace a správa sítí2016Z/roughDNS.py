#!/usr/bin/env python

import sys
import dns.zone

from dns.exception import DNSException
from dns.rdataclass import *
from dns.rdatatype import *
# roughDNS.py zonefile address Type

zonefile=sys.argv[1]
adresa=sys.argv[2]
typeName=sys.argv[3]

f = open(zonefile, 'r')

content = f.read()

if content[0]!=';':
	print('ERROR')
else:
	content= "$ORIGIN" + content[1:]
	content= content[:content.index("\n")] + '.' + content[content.index("\n"):]
	#zone=dns.zone.from_text(content,origin=adresa)
	zone =""
	typ=0
	ttl1=""
	notFound=1
	adresa+='.'
	list = []
	list.append(adresa)
	while len(list)>0 :
		subdomain=""
		domain=list.pop(0)

		#ak je viac . tak musime zistit ci sa nenachadza zaznam vo vyssej domene
		a=domain[:-1]
		#print a
		count =a.count('.')
		#domain+='.'
		Asplit=a.split('.')

		i=0
		found=0
		while i<count and found==0:
			d=1
			subdomain=""
			for x in xrange(0,i):
				subdomain=subdomain+Asplit[x]+'.'
			subdomain=subdomain[:-1]
			adresa1=""
			for x in xrange(count,i-1,-1):
				adresa1=Asplit[x]+'.'+adresa1
			#testovacie vystupy
			#print("adresa1:"+adresa1)
			#print("\tsubdomain:"+subdomain)
			i+=1
			try:
				zone = dns.zone.from_text(content, adresa1)
				typ=dns.rdatatype.from_text(typeName)
			except DNSException, e:
				d=0

			if d==1:
				#print "Zone origin:", zone.origin
				for name, node in zone.nodes.items():
					#rdatasets = node.rdatasets
					#"\n**** BEGIN NODE ****"
					#print "node name:", name
					for rdataset in node.rdatasets:
						#"--- BEGIN RDATASET --------------------------------"
						#print "rdataset string representation:", rdataset
						#print "rdataset rdclass:", rdataset.rdclass
						#print "rdataset rdtype:", rdataset.rdtype
						ttl=rdataset.ttl
						#print "rdataset has following rdata:"
					 	for rdata in rdataset:
							# "-- BEGIN RDATA +++++++++++++++++++++++++++++++"
							#print name
							strName=str(name)
							#print("Name:"+strName)
							if strName=="@":
								strName=adresa1;

							if not strName.endswith('.'):
								strName+='.'+adresa1

							#print("strName:"+strName)
							#print("domain:"+domain)


							if strName == domain:
							



								#ak mame zaznam cname museme rekurzivne hladat aj v kanonickej adrese
								if rdataset.rdtype ==CNAME:
									target=str(rdata.target)
									if target=="@":
										target=adresa1
									adr=""
									if target[-1]=='.':
										adr=target
									else:
										adr=target+"."+adresa1

									#adr=( target if target[-1]=='.' else target+"."+adresa1)
									list.append(adr)




								#print "rdata string representation:", rdata
								if rdataset.rdtype ==typ:
									print "OK"
									found=1
									notFound=0
									#print "dvvdv"
									if rdataset.rdtype == SOA:
										# "** SOA-specific rdata **"
										mname=str(rdata.mname)
										if mname=="@":
											mname=adresa1
										print( mname if mname[-1]=='.' else mname+"."+adresa1)
										#print  #mname:
										#print rdata.rname #rname:
										rname=str(rdata.rname)
										if rname=="@":
											rname=adresa1
										print( rname if rname[-1]=='.' else rname+"."+adresa1)
										print rdata.serial #serial:
										print rdata.refresh #refresh:
										print rdata.retry #retry:
										print rdata.expire #expire:
										print rdata.minimum #minimum:
										ttl1=ttl
									if rdataset.rdtype == A:
										# "** A-specific rdata **"
										print rdata.address #address:
										ttl1=ttl
									if rdataset.rdtype == MX:
										# "** MX-specific rdata **"
										#print rdata.exchange #exchange:
										exchange=str(rdata.exchange)
										if exchange=="@":
											exchange=adresa1
										print( exchange if exchange[-1]=='.' else exchange+"."+adresa1)
										print rdata.preference #preference:
										ttl1=ttl
									if rdataset.rdtype == NS:
										# "** NS-specific rdata **"
										#print rdata.target #target:
										target=str(rdata.target)
										if target=="@":
											target=adresa1
										print( target if target[-1]=='.' else target+"."+adresa1)
										ttl1=ttl
									if rdataset.rdtype == AAAA:
										# "** AAAA-specific rdata **"
										print rdata.address #address:
										ttl1=ttl
									if rdataset.rdtype == CNAME: 
										# "** CNAME-specific rdata **"
										#print rdata.target #target:
										target=str(rdata.target)
										if target=="@":
											target=adresa1
										print( target if target[-1]=='.' else target+"."+adresa1)
										ttl1=ttl
									if rdataset.rdtype == PTR:
										# "** PTR-specific rdata **"
										#print rdata.target #target:
										target=str(rdata.target)
										if target=="@":
											target=adresa1
										print( target if target[-1]=='.' else target+"."+adresa1)
										ttl1=ttl
									if rdataset.rdtype == TXT:
										# "** TXT-specific rdata **"
										print len(rdata.strings) #count of strings
										for txt_string in rdata.strings:
											print txt_string #TXT:
										ttl1=ttl
									print ttl1 #rdataset ttl:
	
	
	#ak sa nenasiel zaznam tak odosleme spravu o nenajdeni
	if notFound==1:
		print "NOT_FOUND"