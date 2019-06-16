<?php
$debug=0;
#SYN:xondre08
function Help() {
    echo "Napoveda pre skript syn.php\n".
		"--help alebo -h\t= vypise tuto napovedu\n".
		"--br\t= za kazdy riadok prida <br />\n".
		"--input=xxx alebo -i=xxx\t= vstupny subor xxx\n".
		"--output=xxx alebo -o=xxx\t= vystupny subor xxx\n".
		"--format=xxx alebo -f=xxx\t= formatovaci subor\n";
}
    $ChybaArgumentov=0;
	$ChybaVstup=0;
	$ChybaVystup=0;
	$ChybaVAplikovaniFormatovacichPrikazov=0;
	$CHYBA=0;
	//zadane argumenty
    $BR=0;
	$formatZadany=0;
	$inputZadany=0;
	$outputZadany=0;
	//
	$input=NULL;
	$output=NULL;
	$format=NULL;
	//otvorene subory
	$inputO=0;
	$outputO=0;
	$formatO=0;
    //HELP
	$helpEn=0;
	//premenne do ktorych sa ulozi cesta k suboru
	$fOutput="";
	$fInput="";
	$fFormat=0;
	 $STDERR = fopen('php://stderr', 'w+');
	$inputEmpty=0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////Praca so subormy////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if($argc==2)// za help-om nemoze nasledovat ziadny iny argument
	{
		if($argv[1]=='--help' || $argv[1]=='-h')// vypis napovedy
		{
			$helpEn=1;
			
			Help();
			exit(0);
		}
	}
	if($helpEn==0 && $argc>=1 && $argc<=5)
	{
		if($debug==1)
		{
			echo "\n\npocet argumentov je ".$argc.".\n";
		}
		
		
		for ($cisloArgumentu = 1; $cisloArgumentu <  $argc; $cisloArgumentu++) 
		{
			if($debug==1)
			{
				echo "nacitanie " . $cisloArgumentu . " argumentu" . "\n";
			}
			
			if($ChybaArgumentov==1 || $ChybaVstup==1 || $ChybaVystup==1)
			{//nastala chyba, nieje dovod pokracovat dalsimi parametrami 
				if($debug==1)
				{
					echo "nastala chyba, nepokracujeme dalsimi argumentami, spracovanych ".$cisloArgumentu." z ". $argc." \n";
				}
				break;
			}
			elseif($argv[$cisloArgumentu] == '-b' || $argv[$cisloArgumentu] == '--br')
			{
				if($debug==1)
				{
					echo "Je nastavene BR.\n";
				}
				$BR=1;
			}
			elseif(strpos($argv[$cisloArgumentu],'--format=')===0 || strpos($argv[$cisloArgumentu],'-f=')===0)
			{
				//nemusi byt zadany , v tom pripade je: vystup=vstup - $formatO==0
				if($debug==1)
				{
					echo "Format:\n";
				}
				
				if($formatZadany!=0)
				{
					$ChybaArgumentov=1;
					if($debug==1)
					{
						echo "Chyba - Format uz bol zadany\n";
					}
				}
				else
				{
					$formatZadany=1;
					//vystrihnutie suboru
					if(strpos($argv[$cisloArgumentu],'--format=')===0)
					{
						$fFormat=substr($argv[$cisloArgumentu],9);
					}
					else //v skratenom tvare -f=
					{
						$fFormat=substr($argv[$cisloArgumentu],3);
					}
					if($debug==1)
					{
						echo "Format: nazov suboru je " . $fFormat . "\n";
					}
					if(file_exists($fFormat))
					{
						$format = fopen($fFormat, "r");//otvorenie suboru pre vstup
						$formatO = 1;
						if(0 == filesize( $fFormat ))//subor je prazdny
						{
							if($debug==1)
							{
								echo "Format: subor je prazdny\n";
							}
							$formatO = 0;
							fclose($format);
						}
					}
					else
					{
						if($debug==1)
						{
							echo "Format: nepodarilo sa otvorit subor\n";
						}
						$ChybaArgumentov=1;
					}
				}
			}
			elseif(strpos($argv[$cisloArgumentu],'--input=')===0 || strpos($argv[$cisloArgumentu],'-i=')===0)
			{
				if($debug==1)
				{
					echo "Input:\n";
				}
				if($inputZadany!=0)
				{
					$ChybaArgumentov=1;
					if($debug==1)
					{
						echo "Chyba - Input uz bol zadany\n";
					}
				}
				else
				{
					$inputZadany=1;
					//vystrihnutie suboru
					if(strpos($argv[$cisloArgumentu],'--input=')===0)
					{
						$fInput=substr($argv[$cisloArgumentu],8);
					}
					else //v skratenom tvare -i=
					{
						$fInput=substr($argv[$cisloArgumentu],3);
					}
					if($debug==1)
					{
						echo "Input: nazov suboru je " . $fInput . "\n";
					}
					if(/*$input = fopen($fInput, "r");*/file_exists($fInput))
					{
						$input = fopen($fInput, "r");//otvorenie suboru pre vstup
						$inputO = 1;
					}
					else
					{
						fwrite($STDERR,  "File `".$fInput."' does not exist.\n");
							
						
						$ChybaVstup=1;
					}
				}
			}
			elseif(strpos($argv[$cisloArgumentu],'--output=')===0 || strpos($argv[$cisloArgumentu],'-o=')===0)
			{
				if($debug==1)
				{
					echo "Output:\n";
				}
				if($outputZadany!=0)
				{
					if($debug==1)
					{
						echo "Chyba - Output uz bol zadany\n";
					}
					$ChybaArgumentov=1;
				}
				else
				{
					$outputZadany=1;
					//vystrihnutie suboru
					if(strpos($argv[$cisloArgumentu],'--output=')===0)
					{
						$fOutput=substr($argv[$cisloArgumentu],9);
					}
					else //v skratenom tvare -o=
					{
						$fOutput=substr($argv[$cisloArgumentu],3);
					}
					if($debug==1)
					{
						echo "Output: nazov suboru je " . $fOutput . "\n";
					}
					if((is_writable($fOutput)||!file_exists($fOutput)) && is_dir(dirname($fOutput)))
					{
						$output = fopen($fOutput, "w");
						//;//otvorenie suboru pre vystup
						$outputO = 1;
					}
					else
					{
						if($debug==1)
						{
							echo "Output: nepodarilo sa otvorit subor\n";
						}
						$ChybaVystup=1;
					}
				}
			}
			else
			{
					fwrite($STDERR,  "Invalid argument `".$argv[$cisloArgumentu]."'.\n");
					//fwrite($STDERR,  "Format table error: Nonexistent parameter `foobar'\n");
					$ChybaArgumentov=1;
			}
		} 
		
		if($ChybaArgumentov==0 && $ChybaVstup==0 && $ChybaVystup==0)//nenastala chyba v argumentoch - mozeme zacat spracovanie  :)
		{
			//ak bol zadany vstup, tak sa obsah prekopiruje do premennej
			if($debug==1)
			{
				echo "Vstup " . ($inputZadany==1? "zo suboru" : " z stdin\n");
			}
			if($inputO==1)
			{
				if(filesize($fInput)==0)
				{
					$inputEmpty=1;
					$textNaFormatovanie=" ";
				}
				else
				$textNaFormatovanie= fread($input,filesize($fInput));//file_get_contents($fInput);
			}
			else
			{
				$textNaFormatovanie=file_get_contents("php://stdin");
			}
			
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////Regularny vyraz ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if($formatO==1 )//ak nebol zadany format, obsah vstupneho suboru sa ulozi do vystupneho suboru.Ak bol, tak prejde upravamy
			{
				if($debug==1)
				{
					echo "Spracovanie \n";
				}

				$poleNajdenychPozicia=array();
				$poleNajdenychTyp=array();
				while(!feof($format))//postupne nacitanie riadkov v subore format
				{
					if($debug==1)
					{
						echo "Nacitanie dalsieho formatovacieho riadku. \n";
					}
					//zobereme cely riadok
					$lines = fgets($format);
					if($lines!="\n"&&$lines!="")//musime zistit ci riadok nieje prazdny
					{
						if(strpos($lines, "\t") !== false)//musi obsahovat 
						{
							//Regulárny vyraz
							$line=explode("\t",$lines,2);
							if(count($line)==2 && $line[0]!="" && $line[1]!="")
							{
								$regularExp=$line[0];
								$zoznamFormatovacichPrikazov=explode(',',str_replace(array(" ", "\t","\n"),"",$line[1]));//odstranime medzery a tabulatory a napokon rozdelime na jednotlive prikazy
								if($debug==1)
								{
									echo "Nacitany regularny vyraz: ".$line[0]."\n";
									echo "Formatovacie prikazy:";
									print_r($zoznamFormatovacichPrikazov);
								}
								//zmena regularneho vyrazu zo zadaneho typu na typ php
								$regularExpression="";
								$negacia=0;
								$pocetZatvoriek=0;
								$pomNegacia="";
								$pomPremenna=0;//pouzita na pocet znakov pri negacii (max 1), znak | vynuluje
								$pomNegacia1=0;
								$ppp=0;
								$poleNeg=array();
								$poleZatvoriek=array();
								$pocZnakov=0;
								$lubovolnyZnakVNegacii=0;
								//test ze sa nezakoncuje negaciu
								if(strlen($regularExp)>0&&$regularExp[strlen($regularExp)-1]=='!')
								{
									if(strlen($regularExp)>1&&$regularExp[strlen($regularExp)-2]=='%')
									{
										
									}
									else
									$CHYBA=1;
								}
								for($i=0;$i<strlen($regularExp);$i++)
								{
									$pomNegacia1=$negacia;
									$pomPremenna++;
									//$negacia--;
									$ppp=0;
									$pocZnakov++;
									if($debug==1)
									{
										echo "1. \t".$negacia. "\t". $regularExp[$i] ."\n";
									}
									switch($regularExp[$i])
									{
										case '%':
											$i++;
											if($i<strlen($regularExp))
											{
												switch($regularExp[$i])
												{
													case "s": // - biele znaky ( \t\n\r\f\v)
														$regularExpression=$regularExpression.'\s';
														break;
													case "a": // - jeden libovolný znak //musime testovat ci nieje v negacii - regex preskocime
														$regularExpression=$regularExpression.'(.|\n)';
														if($pomNegacia1%2!=0)
														{
															$lubovolnyZnakVNegacii=1;
														}
														break;
													case "d": // - čísla od 0 do 9
														$regularExpression=$regularExpression.'[0-9]';
														break;
													case "l": // - malá písmena od a do z
														$regularExpression=$regularExpression.'[a-z]';
														break;
													case "L": // - velká písmena od A do Z
														$regularExpression=$regularExpression.'[A-Z]';
														break;
													case "w": // - malá a velká písmena, tj. (%l|%L)
														$regularExpression=$regularExpression.'([A-Z]|[a-z])';
														break;
													case "W": // - všechna písmena a čísla, tj. (%w|%d)
														$regularExpression=$regularExpression.'([A-Z]|[a-z]|[0-9])';
														break;
													case "t": // - znak tabulátoru (\t)
														$regularExpression=$regularExpression.'\t';
														break;
													case "n": // - znak nového řádku (\n)
														$regularExpression=$regularExpression.'\n';
														break;
													//specialne symboly
													case ".":
														$regularExpression=$regularExpression.'\.';
														break;
													case "|":
														$regularExpression=$regularExpression.'\|';
														break;
													case "!":
														$regularExpression=$regularExpression.'\!';
														break;
													case "*":
														$regularExpression=$regularExpression.'\*';
														break;
													case "+":
														$regularExpression=$regularExpression.'\+';
														break;
													case "(":
														$regularExpression=$regularExpression.'\(';
														break;
													case ")":
														$regularExpression=$regularExpression.'\)';
														break;
													case "%":
														$regularExpression=$regularExpression.'\%';
														
														break;
													default:
														$CHYBA=1;
														break;
												}
											}

										break;
										
										case ".":
											if($i!=0)//pred * musi byt platny znak , regex sa nemoze zacinat znakom *
											{
												for($counter=$i;;$counter--)
												{
													if($counter==0||in_array($regularExp[$counter-1],array("(","|","!",".")))
													{
														$CHYBA=1;
														break;
													}
													else
													{
														if($regularExp[$counter-1]!=")")
														{
															break;	
														}
													}
												}
											}
											else
											{
												$CHYBA=1;
											}
											$pomPremenna--;
											if($i+1<strlen($regularExp))
												//musim zistit ci po znaku '.' nasleduje alfanumericky znak
											{
												if(!($regularExp[$i+1]!="|"&&$regularExp[$i+1]!="*"&&$regularExp[$i+1]!="+"&&$regularExp[$i+1]!="."))
												{
													$CHYBA=1;
												}
											}
											else
											{
												$CHYBA=1;
											}
											break;
										case "|":
											$regularExpression=$regularExpression."|";
											$pomPremenna--;
											$pocZnakov--;
											if($pocZnakov==0)
											{
												$CHYBA=1;
											}
											$pocZnakov=0;
											if($negacia>0)
											{
												if($pomPremenna==1)
												{
													
													$pomPremenna=0;
												}
												else
												{
													$CHYBA=1;
												}
											}
											break;
										case "\\":
											$regularExpression=$regularExpression."\\\\";
											break;
										case "!":
											$pomPremenna--;
											$pocZnakov--;
											array_push($poleNeg,$pomPremenna+1);
											$pomPremenna=0;
								$ppp=1;
											if($pomNegacia1%2!=0)
											{
												//postupne prejdeme vyraz od konca, ci nemame prazdnu zatvorku
												$prazdna=0;
												for($kk=strlen($regularExpression)-1;;$kk--)
												{
													if($regularExpression[$kk]=='^'&&$regularExpression[$kk-1]=='[')
													{
														if($prazdna==0)//treba vymazat
														{
															$regularExpression=substr_replace($regularExpression,"",$kk-1,2);
														}
														else
														{
															$regularExpression=$regularExpression."]";
														}
														break;
													}
													if($regularExpression[$kk]!=')'&&$regularExpression[$kk]!='(')
													{
														$prazdna++;
													}
												}
												
											}
											else
											{
												$regularExpression=$regularExpression."[^";
											}
											$negacia++;
											$pomNegacia=$pomNegacia."!";
											break;
										case "(":
											$regularExpression=$regularExpression."(";
											$pocetZatvoriek++;
											$pomNegacia=$pomNegacia."(";
											$pomPremenna--;
											array_push($poleZatvoriek,$pocZnakov+1);
											$pocZnakov=0;
											break;
										case ")":
											$regularExpression=$regularExpression.')';
											$pocetZatvoriek--;
											$pomPremenna--;
											$negacia--;
											if($pocZnakov==1)//medzi zatvorkami sa nenachadza ziadny znak (1 kvoli tomu ze aj koniec zatvorky je jeden znak)
											{
												$CHYBA=1;
											}
											$pocZnakov=array_pop($poleZatvoriek);
											if(strlen($pomNegacia)>0&&$pomNegacia[strlen($pomNegacia)-1]=='(')
											//vlozi sa koniec zatvorky
											{
												
												$pomNegacia=substr($pomNegacia, 0, -1);//ostranenie vykricnika
											}
											else
											{
												$CHYBA=1;
											}
											break;
										case "*":
											if($i!=0)//pred * musi byt platny znak , regex sa nemoze zacinat znakom *
											{
												for($counter=$i;;$counter--)
												{
													if($counter==0||in_array($regularExp[$counter-1],array("(","|","!",".")))
													{
														$CHYBA=1;
														break;
													}
													else
													{
														if($regularExp[$counter-1]!=")")
														{
															break;	
														}
													}
												}
											}
											else
											{
												$CHYBA=1;
											}
											while($i+1<strlen($regularExp)&& ($regularExp[$i+1]=='+' || $regularExp[$i+1]=='*'))
											{
												$i++;
											}
											$regularExpression=$regularExpression.'*';
											break;
										case "+":
											if($i!=0)//pred * musi byt platny znak , regex sa nemoze zacinat znakom *
											{
												for($counter=$i;;$counter--)
												{
													if($counter==0||in_array($regularExp[$counter-1],array("(","|","!",".")))
													{
														$CHYBA=1;
														break;
													}
													else
													{
														if($regularExp[$counter-1]!=")")
														{
															break;	
														}
													}
												}
											}
											else
											{
												$CHYBA=1;
											}
											$pocet=1;
											while($i+1<strlen($regularExp)&& ($regularExp[$i+1]=='+' || $regularExp[$i+1]=='*'))
											{
												$i++;
												if($regularExp[$i]=='+' && $pocet!=-1)
												{
													$pocet++;
												}
												if($regularExp[$i]=='*')
												{
													$pocet=-1;
												}
											}
											if($pocet==-1)
											{
												$regularExpression=$regularExpression.'*';
											}
											else
											{
												$regularExpression=$regularExpression."+";
											}
											break;
										default:
											if (ord($regularExp[$i])>=32)
											{
												if((ord($regularExp[$i])>=48 && ord($regularExp[$i])<=57)||(ord($regularExp[$i])>=65&&ord($regularExp[$i])<=90)||(ord($regularExp[$i])>=97&&ord($regularExp[$i])<=122))//znaky 0-9 A-Z a-z
												{
													$regularExpression=$regularExpression.$regularExp[$i];
												}
												else
												{
													$regularExpression=$regularExpression.'\\';
													$regularExpression=$regularExpression.$regularExp[$i];
												}
											}

											break;
										break;
									}
									if($debug==1)
									{
										echo "2. \t".$negacia. "\t". $regularExp[$i] ."\n";
										echo $pomPremenna."\t".$regularExp[$i]."\n";
									}
									while(/*$negacia>0*/$pomNegacia1>=0&&$ppp==0&&strlen($pomNegacia)>0&&$pomNegacia[strlen($pomNegacia)-1]=='!' )
									//vlozi sa koniec zatvorky
									{ 
										if($debug==1)
										{
											echo $pomNegacia1;
										}
										//$negacia--;
										if($pomPremenna==1)
										{
										//echo "debug\n";
											$pomNegacia=substr($pomNegacia, 0, -1);//ostranenie vykricnika
											if($pomNegacia1%2==0)
											{
												$regularExpression=$regularExpression."[^";
											}
											else
											{
												//postupne prejdeme vyraz od konca, ci nemame prazdnu zatvorku
												$prazdna=0;
												for($kk=strlen($regularExpression)-1;;$kk--)
												{
													if($regularExpression[$kk]=='^'&&$regularExpression[$kk-1]=='[')
													{
														if($prazdna==0)//treba vymazat
														{
															$regularExpression=substr_replace($regularExpression,"",$kk-1,2);
														}
														else
														{
															$regularExpression=$regularExpression."]";
														}
														break;
													}
													if($regularExpression[$kk]!=')'&&$regularExpression[$kk]!='(')
													{
														$prazdna++;
													}
												}
											}
											//$pomPremenna++;
										$pomPremenna=array_pop($poleNeg);
										
										}
										else 
										{
											$CHYBA=1;//chyba: viac ako 1 znak alebo nebol ziadny zadany priklad:[^(a|bd)],[^(a|)],[^(sd|p)],[^(slovo1|slovo2)]
										}
										$pomNegacia1--;
									}
									if($debug==1)
									{
										echo "regularny vyraz je: ".$regularExpression."\n";
									}
									
									if($pocetZatvoriek<0)//ak je zaporny pocet zatvoriek (bolo viac ukoncenych ako zacatych): (()))
									{
										$CHYBA=1;
									}
								}
								if($pocetZatvoriek!=0 || $pocZnakov==0) //pocet zatvoriek ( a ) musi byt rovnaky a nemoze sa koncit znakom |
								{
									$CHYBA=1;
								}
								//test ci nieje negacia na konci - neukoncena negacia
								if(strlen($regularExpression)>=2&&$regularExpression[strlen($regularExpression)-1]=='^'&&$regularExpression[strlen($regularExpression)-2]=='[')
								{
									$CHYBA=1;
								}
								if($CHYBA==1)
								{
									fwrite($STDERR,  "Format table error: Invalid regular expression `".$regularExp."'.\n");
									break;
								}
								
								$regularExpression='/('.$regularExpression.')/';
								if($debug==1)
								{
									echo "Vysledny regularny vyraz je: ".$regularExpression."\n";
								}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////vyhladavanie reg vyrazu/////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//vyhladavanie - najdeny substring sa ulozi do matches 
								$matches=array(array());//vytvorenie pola pre najdene substringy
								if($lubovolnyZnakVNegacii==0) // lubovolny znak v negacii
								{
									preg_match_all($regularExpression,$textNaFormatovanie,$matches);
								}
								if($debug==1)
								{
									echo "Najdene:";
									print_r($matches);
								}
								//$matches[0]=array_filter($matches[0]);
								//$poleNajdenychPozicia=[];
								//$poleNajdenychTyp=[];
								//do pola sa ulozi cislo kde sa nachadza hladany 
								$poziciaOld=0;
								//cyklus na jednotlive typy formatovania
								$k=0;
								for($i=0;$i<count($matches[0]) && $ChybaVAplikovaniFormatovacichPrikazov==0;$i++)
								{
									if($matches[0][$i]=='')
									{
										$k++;
									}
									else
									{
									if($debug==1)
									{
										echo "Hlada sa string:".$matches[0][$i]."\n";
									}
									//pozicia kde sa nasiel
									
									$pozicia=strpos($textNaFormatovanie,$matches[0][$i], $i-$k==0?0:/*($matches[0][$i]==$matches[0][$i-$k-1]?(*/$poziciaOld+strlen($matches[0][$i-$k-1])/*):$poziciaOld)*/);
									/*if($poziciaOld!=0 &&( ($matches[0][$i]==$matches[0][$i-$k-1]?)-$poziciaOld<strlen($matches[0][$i-1])))//riesi prekryvanie toho isteho reg vyrazu
									{
										$poziciaOld=$pozicia;
										continue;
									}*/
									$poziciaOld=$pozicia;
									$position=0;
									$k=0;
									if($debug==1)
									{
										echo "Najdene na pozicii:".$pozicia."\n";
									}
									
									for($j=0;$j<count($zoznamFormatovacichPrikazov);$j++)
									{
										//zaciatok
										if(count($poleNajdenychPozicia)!=0)
										for(;$position<count($poleNajdenychPozicia);$position++)
										{
											if(($pozicia<$poleNajdenychPozicia[$position]))
											{
												
												break;
											}
										}	
										array_splice($poleNajdenychPozicia,$position,0,$pozicia);
										//nasleduje switch v ktorom sa urci ze co sa ma vlozit
									
										if($debug==1)
									{
										echo "Najdene na pozicii:".$pozicia. "-nemoze sa zmenit\n";
									}
										if($debug==1)
										{
											echo "aplikovanie formatovacieho prikazu:" . $zoznamFormatovacichPrikazov[$j]. "\n";
										}
										switch($zoznamFormatovacichPrikazov[$j])
										{
											case "bold":
												array_splice($poleNajdenychTyp,$position,0,"<b>");//vlozi sa zaciatok tagu
												break;
											case "italic":
												array_splice($poleNajdenychTyp,$position,0,"<i>");//vlozi sa zaciatok tagu
												break;
											case "underline":
												array_splice($poleNajdenychTyp,$position,0,"<u>");//vlozi sa zaciatok tagu
												break;
											case "teletype":
												array_splice($poleNajdenychTyp,$position,0,"<tt>");//vlozi sa zaciatok tagu
												break;
											default:
												if(strpos($zoznamFormatovacichPrikazov[$j],'size:')===0)
												{
													$cislo=substr($zoznamFormatovacichPrikazov[$j],5);
													if(strlen($cislo)==1)//cislo>=1 &&cislo<=7
													{
														if($cislo=='1'||$cislo=='2'||$cislo=='3'||$cislo=='4'||$cislo=='5'||$cislo=='6'||$cislo=='7')
														{
															array_splice($poleNajdenychTyp,$position,0,"<font size=".$cislo.">");
														}
														else
														{
															$ChybaVAplikovaniFormatovacichPrikazov=1;
															
															fwrite($STDERR,  "Format table error: Invalid size `".$cislo."'.\n");
															
														}
													}
													else
													{
														fwrite($STDERR,  "Format table error: Invalid size `".$cislo.".'\n");
														//chyba
														$ChybaVAplikovaniFormatovacichPrikazov=1;
														if($debug==1)
														{
															echo "CHYBA: Zvolena velkost ma viac ako 1 znak\n";
														}
													}
												}
												elseif(strpos($zoznamFormatovacichPrikazov[$j],'color:')===0)
												{
													$hex=substr($zoznamFormatovacichPrikazov[$j],6);
													//echo "\"".$hex."\"\n";
													if(strlen($hex)==6)//hex>=000000 && hex<=FFFFFF
													{
														
														foreach(str_split($hex) as $num)
														{
															if($num=='0'||$num=='1'||$num=='2'||$num=='3'||$num=='4'||$num=='5'||$num=='6'||$num=='7'||$num=='8'||$num=='9'||$num=='A'||$num=='B'||$num=='C'||$num=='D'||$num=='E'||$num=='F'||$num=='a'||$num=='b'||$num=='c'||$num=='d'||$num=='e'||$num=='f')
															{
																
															}
															else
															{
																$ChybaVAplikovaniFormatovacichPrikazov=1;
																if($debug==1)
																{
																	echo "CHYBA: Zvolena farba je mimo interval 000000 - FFFFFF\n";
																}
															}
														}
														if($ChybaVAplikovaniFormatovacichPrikazov==0)
														{
															
															array_splice($poleNajdenychTyp,$position,0,"<font color=#".$hex.">");
														}
													}
													else
													{
														//chyba
														$ChybaVAplikovaniFormatovacichPrikazov=1;
														if($debug==1)
														{
															echo "CHYBA: Zvolena farba ma viac ako 6 znakov\n";
														}
													}
												}
												else
												{
													//chyba
													fwrite($STDERR,  "Format table error: Nonexistent parameter `".$zoznamFormatovacichPrikazov[$j].".'\n");
													$ChybaVAplikovaniFormatovacichPrikazov=1;
													if($debug==1)
													{
														echo "CHYBA:Chybny formatovaci prikaz\n";
													}
												}
												break;
										}
										
										//koniec
										$poziciaA=$pozicia+strlen($matches[0][$i]);
										for($position=count($poleNajdenychPozicia)-1;$position>=0;$position--)
										{
											if($poziciaA>$poleNajdenychPozicia[$position])
											{
												$position++;
												break;
											}
										}	
										array_splice($poleNajdenychPozicia,$position,0,$poziciaA);
										switch($zoznamFormatovacichPrikazov[$j])
										{
											case "bold":
												array_splice($poleNajdenychTyp,$position,0,"</b>");//vlozi sa zaciatok tagu
												break;
											case "italic":
												array_splice($poleNajdenychTyp,$position,0,"</i>");//vlozi sa zaciatok tagu
												break;
											case "underline":
												array_splice($poleNajdenychTyp,$position,0,"</u>");//vlozi sa zaciatok tagu
												break;
											case "teletype":
												array_splice($poleNajdenychTyp,$position,0,"</tt>");//vlozi sa zaciatok tagu
												break;
											default:
												if(strpos($zoznamFormatovacichPrikazov[$j],'size:')===0)
												{
													$cislo=substr($zoznamFormatovacichPrikazov[$j],5);
													if(strlen($cislo)==1)//cislo>=0 &&cislo<=7
													{
														if($cislo=='1'||$cislo=='2'||$cislo=='3'||$cislo=='4'||$cislo=='5'||$cislo=='6'||$cislo=='7')
														{
															array_splice($poleNajdenychTyp,$position,0,"</font>");
														}
														else
														{
															$ChybaVAplikovaniFormatovacichPrikazov=1;
															if($debug==1)
															{
																echo "CHYBA: Zvolena velkost je mimo interval 0 - 7\n";
															}
														}
													}
													else
													{
														//chyba
														$ChybaVAplikovaniFormatovacichPrikazov=1;
														if($debug==1)
														{
															echo "CHYBA: Zvolena velkost ma viac ako 1 znak\n";
														}
													}
												}
												elseif(strpos($zoznamFormatovacichPrikazov[$j],'color:')===0)
												{
													$hex=substr($zoznamFormatovacichPrikazov[$j],6);
													if(strlen($hex)==6)//hex>=000000 && hex<=FFFFFF
													{
														
														foreach(str_split($hex) as $num)
														{
															if($num=='0'||$num=='1'||$num=='2'||$num=='3'||$num=='4'||$num=='5'||$num=='6'||$num=='7'||$num=='8'||$num=='9'||$num=='A'||$num=='B'||$num=='C'||$num=='D'||$num=='E'||$num=='F'||$num=='a'||$num=='b'||$num=='c'||$num=='d'||$num=='e'||$num=='f')
															{
																
															}
															else
															{
																$ChybaVAplikovaniFormatovacichPrikazov=1;
																if($debug==1)
																{
																	echo "CHYBA: Zvolena farba je mimo interval 000000 - FFFFFF\n";
																}
															}
														}
														if($ChybaVAplikovaniFormatovacichPrikazov==0)
														{
															
															array_splice($poleNajdenychTyp,$position,0,"</font>");
														}
													}
													else
													{
														//chyba
														$ChybaVAplikovaniFormatovacichPrikazov=1;
														if($debug==1)
														{
															echo "CHYBA: Zvolena farba ma viac ako 6 znakov\n";
														}
													}
												}
												else
												{
													//chyba
													$ChybaVAplikovaniFormatovacichPrikazov=1;
													if($debug==1)
													{
														echo "CHYBA:Chybny formatovaci prikaz\n";
													}
												}
												break;
										}
										if($debug==1 && $ChybaVAplikovaniFormatovacichPrikazov==0)
										{
											echo "Aplikovanie uspesne\n";
										}
										if($debug==1)
										{
											print_r($poleNajdenychPozicia);
											print_r($poleNajdenychTyp);
										}
									}
									}	
								}
								if(count($matches[0])==0)//musime otestovat ci sa na pozicii formatovania nachadza validny typ
								{
									for($j=0;$j<count($zoznamFormatovacichPrikazov);$j++)
									{
										switch($zoznamFormatovacichPrikazov[$j])
										{
											case "bold":
												break;
											case "italic":
												break;
											case "underline":
												break;
											case "teletype":
												break;
											default:
												if(strpos($zoznamFormatovacichPrikazov[$j],'size:')===0)
												{
													$cislo=substr($zoznamFormatovacichPrikazov[$j],5);
													if(strlen($cislo)==1)//cislo>=0 &&cislo<=7
													{
														if($cislo=='1'||$cislo=='2'||$cislo=='3'||$cislo=='4'||$cislo=='5'||$cislo=='6'||$cislo=='7')
														{
															
														}
														else
														{
															$ChybaVAplikovaniFormatovacichPrikazov=1;
															fwrite($STDERR,  "Format table error: Invalid size `".$cislo."'.\n");
														}
													}
													else
													{
														//chyba
														$ChybaVAplikovaniFormatovacichPrikazov=1;
														fwrite($STDERR,  "Format table error: Invalid size `".$cislo."'.\n");
													}
												}
												elseif(strpos($zoznamFormatovacichPrikazov[$j],'color:')===0)
												{
													$hex=substr($zoznamFormatovacichPrikazov[$j],6);
													if(strlen($hex)==6)//hex>=000000 && hex<=FFFFFF
													{
														
														foreach(str_split($hex) as $num)
														{
															if($num=='0'||$num=='1'||$num=='2'||$num=='3'||$num=='4'||$num=='5'||$num=='6'||$num=='7'||$num=='8'||$num=='9'||$num=='A'||$num=='B'||$num=='C'||$num=='D'||$num=='E'||$num=='F'||$num=='a'||$num=='b'||$num=='c'||$num=='d'||$num=='e'||$num=='f')
															{
																
															}
															else
															{
																$ChybaVAplikovaniFormatovacichPrikazov=1;
																//fwrite($STDERR,  "Format table error: Invalid color `".$hex."'.\n");
															}
														}
														if($ChybaVAplikovaniFormatovacichPrikazov==0)
														{
															
															//array_splice($poleNajdenychTyp,$position,0,"</font>");
														}
														else
														{
															fwrite($STDERR,  "Format table error: Invalid color `".$hex."'.\n");
														}
													}
													else
													{
														//chyba
														$ChybaVAplikovaniFormatovacichPrikazov=1;
														fwrite($STDERR,  "Format table error: Invalid color `".$hex."'.\n");
													}
												}
												else
												{
													//chyba
													$ChybaVAplikovaniFormatovacichPrikazov=1;
													
													fwrite($STDERR,  "Format table error: Nonexistent parameter `".$zoznamFormatovacichPrikazov[$j]."'.\n");
												}
												break;
										}
									}
								}
								//po dokonceny formatovacieho pola mozme vlozit jednotlive formaty
								
							}
							else
							{
								//chyba
								$ChybaVAplikovaniFormatovacichPrikazov=1;
								if($debug==1)
								{
									echo "CHYBA:Riadok vo formatovacom subore neobsahuje \\t\n";
								}
							}
						}
						else
						{
							//chyba
							$ChybaVAplikovaniFormatovacichPrikazov=1;
							if($debug==1)
							{
								echo "CHYBA:Riadok vo formatovacom subore neobsahuje \\t\n";
							}
						}
					}
					else
					{
						if($debug==1)
						{
							echo "Riadok je prazdny\n";
						}
					}
				}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////vkladanie formatovacich pravidiel///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//finalne vlozenie formatovacich pravidiel do textNaFormatovanie
				if($debug==1)
				{
					print_r($poleNajdenychPozicia);
					print_r($poleNajdenychTyp);
				}
				if(count($poleNajdenychTyp)==count($poleNajdenychPozicia))
				{
					for($i=count($poleNajdenychPozicia)-1;$i>=0;$i--)
					{
						$textNaFormatovanie=substr_replace($textNaFormatovanie, $poleNajdenychTyp[$i], $poleNajdenychPozicia[$i], 0);
					}
				}
			}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////<br />//////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if($BR==1)
			{
				/*while ($ch=fgets($input)) //prekopirovanie obsahu input do output
				{
					fputs($output, $BR==1 ? $ch : $ch . "<br />");
					//Ak je povolene BR , tak sa za kazdym riadkom vlozi <br />
				}*/
				if($debug==1)
				{
					echo "\n\nBR: na koniec riadku sa prida <br />\n\n";
				}
				
				$textNaFormatovanie=str_replace("\n","<br />\n",$textNaFormatovanie);
				
				
			}
			if($debug==1)
			{
				echo "\n\nVypis " . ($outputZadany==1? "do suboru\n" : ":\n");
			}
			if($debug==1)
			{
				$textNaFormatovanie="<!DOCTYPE html>\n<html>\n<head>\n<title>Page Title</title>\n</head>\n<body>".$textNaFormatovanie."</body>\n</html>";
			}
			
			
			if($ChybaVAplikovaniFormatovacichPrikazov==0 && $CHYBA==0&&$inputEmpty==0)//ak nenastala chyba pri spracovani
			{
				if($outputO==1)//ak bol zadany vystup tak sa vypise do suboru, inak sa vypise na standardny vystup
				{
					fwrite($output,$textNaFormatovanie);
				}
				else
				{
					echo $textNaFormatovanie;
				}
			}
			
			
		}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////uzatvaranie suborov//////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if($debug==1)
		{
			echo "\nUzatvaranie suborov\n";
		}
		//zatvorenie suborov
		if($outputO==1)
			fclose($output);
		if($inputO==1)
			fclose($input);
		if($formatO==1)
			fclose($format);
	}
	else
	{
		$ChybaArgumentov=1;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////navratove hodnoty chyb/////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	if($ChybaArgumentov==1)//nastala chyba v argumentoch
	{
		//fwrite($STDERR,  "CHYBA: Chyba v argumentoch.\n");
		exit(1);
	}
	if($ChybaVstup==1)//chyba v otvoreni vstupneho suboru
	{
		//fwrite($STDERR,  "CHYBA: Nepodarilo sa otvorit vstupny subor\n");
		exit(2);
	}
	if($ChybaVystup==1)//chyba v otvoreni vstupneho suboru
	{
		fwrite($STDERR,  "Output directory does not exist.\n");
		exit(3);
	}
	if($ChybaVAplikovaniFormatovacichPrikazov==1)
	{
		//fwrite($STDERR,  "CHYBA: Chyba vo formatovacom subore\n");
		exit(4);
	}
	if($CHYBA==1)
	{
		//fwrite($STDERR, "CHYBA: Chyba v regularnom vyraze\n");
		exit(4);
	}
	exit(0);
?>
