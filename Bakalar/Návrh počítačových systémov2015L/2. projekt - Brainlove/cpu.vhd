-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2015 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Michal Ondrejó
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu
	type instrType is (	PtrInc,
					PtrDec,
					Inc,
					Dec,
					whileStart,
					whileEnd,
					putchar,
					getchar,
					push,
					pop,
					nil,
					halt);
	signal instruct:instrType;
	type fsmState is (sidle,
					sFetch0,
					sFetch1,
					sFetch2,
					sPtrInc,
					sPtrDec,
					sInc,
					sInc2,
					sDec,
					sDec2,
					sWhileStart,
					sWhileStart1,
					sWhileStart2,
					sWhileEnd,
					sWhileEnd1,
					sWhileEnd2,
					sWhileEnd3,
					sPutchar,
					sGetchar,
					sPutchar1,
					sGetchar1,
					sPush,
					sPush1,
					sPop,
					sPop1,
					sNil,
					shalt);
	signal CNT_reg:	std_logic_vector(12 downto 0);
	signal CNT_inc:	std_logic;
	signal CNT_dec:	std_logic;
	signal PC_reg:	std_logic_vector(12 downto 0);
	signal PC_inc:	std_logic;
	signal PC_dec:	std_logic;
	signal PTR_reg:	std_logic_vector(12 downto 0);
	signal PTR_inc:	std_logic;
	signal PTR_dec:	std_logic;
	signal TMP:		std_logic_vector(7 downto 0);
	signal MX1:		std_logic;
	signal MX2:		std_logic_vector(1 downto 0);
	signal pstate:	fsmState;
	signal nstate:	fsmState;
begin

 -- zde dopiste vlastni VHDL kod
 
 
 
 --dekodovanie 
 process (DATA_RDATA)
 begin 
	 if(MX1='1')
	 then
	case(DATA_RDATA(7 downto 0)) is
		when X"3E"=> instruct<= PtrInc;
		when X"3C"=> instruct<= PtrDec;
		when X"2B"=> instruct<= Inc;
		when X"2D"=> instruct<= Dec;
		when X"5B"=> instruct<= whileStart;
		when X"5D"=> instruct<= whileEnd;
		when X"2E"=> instruct<= putchar;
		when X"2C"=> instruct<= getchar;
		when X"24"=> instruct<= push;
		when X"21"=> instruct<= pop;
		when X"00"=> instruct<= halt;
		when others => instruct<= nil;
	end case;
	end if;
end process;
 
 
 --MX1
 DATA_ADDR<=PC_reg when (MX1 = '1') else PTR_reg;
 
 --MX2
with MX2 select
	DATA_WDATA <= DATA_RDATA+1	when "00",
	 DATA_RDATA-1	when "01",
	TMP	when "10",
	IN_DATA	when others;

 
 --PC
PC_counter: process (RESET,CLK)
begin
	if(RESET='1') then
		PC_reg <= (others=>'0');
	elsif(CLK'event) and (CLK='1') then
		if(PC_dec='1') and (PC_reg>0) then
			PC_reg<= PC_reg-1;
		elsif(PC_inc='1') then
			PC_reg<= PC_reg+1;
		end if;
	end if;
end process;

 --PTR
PTR_counter: process (RESET,CLK)
begin
	if(RESET='1') 
	then
		PTR_reg<= "1000000000000";
	elsif(CLK'event) and (CLK='1') 
	then
		if (PTR_dec='1') 
		then
			if (PTR_reg=X"1000") 
			then
				PTR_reg <= "1111111111000";
			else 
				PTR_reg <= PTR_reg - 8;
			end if;
		elsif (PTR_inc='1') 
		then
			if (PTR_reg=X"1FF8") 
			then
				PTR_reg <= "1000000000000";
			else 
				PTR_reg <= PTR_reg + 8;
			end if;
        end if;
	end if;
end process;

 --CNT
CNT_Counter: process (RESET,CLK)
begin
	if(RESET='1') then
		CNT_reg<=(others=>'0');
	elsif(CLK'event) and (CLK='1') then
		if(CNT_dec='1') then
			CNT_reg<= CNT_reg-1;
		elsif(CNT_inc='1') then
			CNT_reg<= CNT_reg+1;
		end if;
	end if;
	
end process;


fsm_PState: process (RESET,CLK)
begin
	if (RESET='1') then
		pstate <= sidle;
	elsif (CLK'event) and (CLK='1')  and (EN = '1') then
		pstate<=nstate;
	end if;
end process;

FSM: process (OUT_BUSY,IN_VLD,IN_DATA,DATA_RDATA,instruct,CNT_reg,pstate)
begin
	--initialisation
	DATA_EN <= '0';
	DATA_RDWR <= '1';
	IN_REQ <= '0';
	OUT_DATA <= X"00";
	OUT_WE <= '0';
	PC_inc<='0';
	PC_dec<='0';
	CNT_inc<='0';
	CNT_dec<='0';
	PTR_inc<='0';
	PTR_dec<='0';
	MX1<='1';
	case pstate is 
	when sidle =>
		nstate<=sFetch0;
		DATA_EN<='1';
	when sFetch0=>
		DATA_EN<='1';
		nstate<=sFetch1;	
	when sFetch1=>
		PC_inc<='1';
		case instruct is
		when PtrInc=>
			nstate<=sPtrInc;
		when PtrDec=>
			nstate<=sPtrDec;
		when Inc=>
			nstate<=sInc;
		when Dec=>
			nstate<=sDec;
		when whileStart=>
			nstate<=sWhileStart;
		when whileEnd=>
			nstate<=sWhileEnd;
		when putchar=>
			nstate<=sPutchar;
		when getchar=>
			nstate<=sGetchar;
		when push=>
			nstate<=sPush;
		when pop=>
			nstate<=sPop;
		when nil=>
			nstate<=sNil;
		when halt=>
			nstate<=shalt;
		when others =>
			nstate<=sFetch0;
		end case;
	when shalt =>
		nstate<=shalt;
	when sInc =>
		DATA_RDWR<='1';
		MX1<='0';
		MX2<="00";
		DATA_EN<='1';
		nstate<=sInc2;
	when sInc2 =>
		DATA_RDWR<='0';
		DATA_EN<='1';
		MX1<='0';
		MX2<="00";
		nstate<=sFetch0;
		
	when sDec =>
		DATA_RDWR<='1';
		MX1<='0';
		MX2<="01";
		DATA_EN<='1';
		nstate<=sDec2;
	when sDec2 =>
		DATA_RDWR<='0';
		DATA_EN<='1';
		MX1<='0';
		MX2<="01";
		nstate<=sFetch0;
	when sPtrInc =>
		nstate<=sFetch0;
		MX1<='0';
		PTR_inc<='1';
	when sPtrDec =>
		nstate<=sFetch0;
		MX1<='0';
		PTR_dec<='1';
	when sWhileStart =>
		--zistenie hodnoty aktualneho bloku
		DATA_EN<='1';
		DATA_RDWR<='1';
		MX1<='0';
		nstate<= sWhileStart1;
	when sWhileStart1 =>
		if(DATA_RDATA=0)
		then
				PC_inc<='1';
				MX1<='1';
				DATA_EN<='1';
				nstate<=sWhileStart2;
			
		else
			nstate<=sFetch0;
		end if;
	when sWhileStart2 =>
		if(instruct=halt)then
			nstate<=shalt;
		elsif(instruct=whileEnd)and(CNT_reg=0) 
		then--posun az za ]
			nstate<=sFetch0;
		else 
			PC_inc<='1';
			MX1<='1';
			DATA_EN<='1';
			nstate<=sWhileStart2;
			if(instruct=whileStart)
			then
				CNT_inc<='1';
			elsif(instruct=whileEnd)
			then
				CNT_dec<='1';
			end if;
		end if;
		
	
	when sWhileEnd =>
			PC_dec<='1';
			MX1<='1';
			DATA_EN<='1';
		nstate<=sWhileEnd2;
	when sWhileEnd2 =>
			PC_dec<='1';
			MX1<='1';
			DATA_EN<='1';
		nstate<=sWhileEnd3;
	when sWhileEnd3 =>
			PC_dec<='1';
			MX1<='1';
			DATA_EN<='1';
		nstate<=sWhileEnd1;
	when sWhileEnd1 =>
		if(instruct/=whileStart)and(PC_reg=X"0000")
		then
			nstate<=shalt;
		elsif(instruct=whileStart)and(PC_reg=X"0000")and(CNT_reg/=0)
		then
			nstate<=shalt;
		
		elsif(instruct=whileStart)and(CNT_reg=0)
		then
			PC_inc<='1';
			nstate<= sFetch0;
		else
			PC_dec<='1';
			MX1<='1';
			DATA_EN<='1';
			nstate<= sWhileEnd1;
			if(instruct=whileStart)
			then
				CNT_dec<='1';
			elsif(instruct=whileEnd)
			then
				CNT_inc<='1';
			end if;
		end if;
	when sPutchar =>
		MX1<='0';
		
		DATA_EN<='1';
		DATA_RDWR<='1';
		nstate<= sPutchar1;
	when sPutchar1 =>
		MX1<='0';
		if(OUT_BUSY='0')
		then
			OUT_DATA<=DATA_RDATA;
			OUT_WE<='1';
			MX1<='1';
			nstate<= sFetch0;
		else
			nstate<= sPutchar1;
		end if;
	when sGetchar =>
		IN_REQ<='1';
		nstate<= sGetchar1;
	when sGetchar1 =>
		if(IN_VLD='1')
		then
			DATA_EN<='1';
			MX2<="11";
			DATA_RDWR<='0';
			nstate<= sFetch0;
			MX1<='0';
		else
			nstate<= sGetchar1;
		end if;
	when sPush =>
		DATA_EN<='1';
		DATA_RDWR<='1';
		MX1<='0';
		nstate<= sPush1;
	when sPush1 =>
		MX1<='0';
		TMP<=DATA_RDATA;
		nstate<= sFetch0;
	when sPop =>
		DATA_EN<='1';
		MX1<='0';
		MX2<="10";
		DATA_RDWR<='0';
		nstate<= sPop1;
	when sPop1=>
		nstate<= sFetch0;
	when others =>
		nstate<= sFetch0;
	end case;
end process;
 
	
end behavioral;
 
