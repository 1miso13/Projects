--Návrh poèítaèových systémù
--projekt è.1: Rizeni maticového displeje z FPGA
--autor: Michal Ondrejó
--login: xondre08
--roèník: 2015/2016
----------------------------------------------


library IEEE;
    use IEEE.std_logic_unsigned.all;
    use IEEE.std_logic_1164.all;
    use IEEE.std_logic_arith.all;

-- Definice rozhrani pro praci s maticovym displejem
entity ledc8x8 is
    port ( RESET: in std_logic;
           SMCLK: in std_logic;
           LED: out std_logic_vector(0 to 7);
           ROW: out std_logic_vector(7 downto  0)
						);
end ledc8x8;


architecture display of ledc8x8 is
	signal ActRow: std_logic_vector(7 downto 0);
	signal Counter: std_logic_vector(21 downto 0);
	signal Delenie_Counter: std_logic_vector(7 downto 0);
	signal Waiter: std_logic;
	signal RotReg: std_logic;
begin
--delenie frekvencie
    delenieCLK:process(RESET,SMCLK)
    begin
        if (RESET='1') then
		Delenie_Counter <= "00000000" ;
        elsif SMCLK'event and (SMCLK='1') then
            Delenie_Counter <=  Delenie_Counter+1;
            if(Delenie_Counter = "11111111") then
                RotReg <= '1';
            else
                RotReg <= '0';
            end if;
        end if;
    end process;
--Oneskorenie priezviska	
	Delay: process(RESET,SMCLK)
    begin
        if (RESET='1') then
            Counter <= "0000000000000000000000" ;
			Waiter <= '0';
        elsif SMCLK'event and (SMCLK='1') and (Waiter='0') then
            Counter <=  Counter+1;
			if(Counter = "1111010000100100000000") then 
				Waiter <= '1';
			end if;
        end if;
    end process;
	
    RotaciaRiadkov: process(RESET,SMCLK,ActRow)
    begin
	if (RESET='1') then
            ActRow <= "10000000" ;
	 elsif  ( SMCLK'event and SMCLK='1'  and RotReg = '1' ) then

            case ActRow is
				when "10000000" =>  ActRow <=  "01000000";
                when "01000000" =>  ActRow <=  "00100000";
                when "00100000" =>  ActRow <=  "00010000";
                when "00010000" =>  ActRow <=  "00001000";
                when "00001000" =>  ActRow <=  "00000100";
                when "00000100" =>  ActRow <=  "00000010";
                when "00000010" =>  ActRow <=  "00000001"; 
                when "00000001" =>  ActRow <=  "10000000";
               
            
                when others   =>  ActRow <=    "00000000"; 
            end case ActRow;
       end if;  
    ROW <= ActRow;  
    end process; 

	
	puttingLEDsOn: process(SMCLK,RESET,ActRow)
    begin
		if(Waiter /= '1')
		then
			case ActRow is

				when "00000001" =>  LED <=   "01110111";
				when "00000010" =>  LED <=   "00100111";
				when "00000100" =>  LED <=   "01010111";
				when "00001000" =>  LED <=   "01110111";
				when "00010000" =>  LED <=   "11111111";
				when "00100000" =>  LED <=   "11111111";
				when "01000000" =>  LED <=   "11111111";
				when "10000000" =>  LED <=   "11111111"; 
				when others     =>  LED <=   "00000000"; 
			end case ;
		else
			case ActRow is

				when "00000001" =>  LED <= "01110111";
				when "00000010" =>  LED <= "00100111";
				when "00000100" =>  LED <= "01010111";
				when "00001000" =>  LED <= "01110111";
				when "00010000" =>  LED <= "11111001";
				when "00100000" =>  LED <= "11110110";
				when "01000000" =>  LED <= "11110110";
				when "10000000" =>  LED <= "11111001"; 
				when others     =>  LED <= "00000000"; 
			end case ;
		end if; 
    end process;
    

end architecture display;
