-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST1, PRINT_MESSAGE, FINISH, AB2, AB3, AB4, AB5, AB6, AB7, A8, A9, A10, B8, B9, B10, PRINT_MESSAGE_OK, testFinal, eee);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1 =>
      next_state <= TEST1;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	  elsif (KEY(1) = '1') then
		next_state <=AB2;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	when AB2 =>
      next_state <= AB2;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	elsif (KEY(5) = '1') then
		next_state <=AB3;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	  when AB3 =>
      next_state <= AB3;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	elsif (KEY(9) = '1') then
		next_state <=AB4;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	  when AB4 =>
      next_state <= AB4;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	elsif (KEY(5) = '1') then
		next_state <=AB5;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	  when AB5 =>
      next_state <= AB5;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	elsif (KEY(1) = '1') then
		next_state <=AB6;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	  when AB6 =>
      next_state <= AB6;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	elsif (KEY(5) = '1') then
		next_state <=AB7;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	  when AB7 =>
      next_state <= AB7;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	elsif (KEY(0) = '1') then
		next_state <=A8;
	elsif (KEY(7) = '1') then
		next_state <=B8;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	  when A8 =>
      next_state <= A8;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	elsif (KEY(8) = '1') then
		next_state <=A9;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	  when A9 =>
      next_state <= A9;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	elsif (KEY(7) = '1') then
		next_state <=A10;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	  when A10 =>
      next_state <= A10;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	elsif (KEY(2) = '1') then
		next_state <=testFinal;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	  when B8 =>
      next_state <= B8;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	elsif (KEY(1) = '1') then
		next_state <=B9;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	  when B9 =>
      next_state <= B9;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	elsif (KEY(8) = '1') then
		next_state <=B10;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	  when B10 =>
      next_state <= B10;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
	elsif (KEY(0) = '1') then
		next_state <=testFinal;
	elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
      end if;
	  
	  
	 when testFinal =>
      next_state <= testFinal;
      if (KEY(14 downto 0) /= "000000000000000") then
         next_state <= eee; 
	elsif (KEY(15) = '1') then
		next_state <=PRINT_MESSAGE_OK;
      end if; 
	 
	 when PRINT_MESSAGE_OK =>
      next_state <= PRINT_MESSAGE_OK;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
	 
	 when eee =>
      next_state <= eee;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE =>
      next_state <= PRINT_MESSAGE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST1; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when AB2 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when AB3 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when AB4 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when AB5 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when AB6 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when AB7 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when A8 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when A9 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when A10 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when B8 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when B9 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when B10 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when TEST1 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when testFinal =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   when eee =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
	
	when PRINT_MESSAGE_OK =>
	FSM_MX_MEM     <= '1';
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
   end case;
end process output_logic;

end architecture behavioral;

