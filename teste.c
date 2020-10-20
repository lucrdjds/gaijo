library IEEE;
use IEEE.Std_Logic_1164.all;

entity sequence is
port (
 S: in std_Logic_vector(3 downto 0);

 Control: in std_logic_vector(1 downto 0);
 Out_hex: out std_logic_vector(6 downto 0);
 Out_bin: out std_logic_vector(3 downto 0);
 
 enter: in std_logic;
 clock: in std_logic;
 reset: in std_logic
 );
 
end entity;
architecture circuito of sequence is

component decod is
port (C: in std_logic_vector(3 downto 0);
 F: out std_logic_vector(6 downto 0)
 );
end component;

component registrador is
port (CLK, RST: in std_logic;
D: in std_logic_vector(3 downto 0);
EN: in std_logic;
Q: out std_logic_vector(3 downto 0)); 
end component;
 signal bin: std_logic_vector(3 downto 0);
 signal qprox: std_logic_vector(3 downto 0);
 begin
	bin(3) <= (( S(2) and (not S(1))) or (S(2) AND (NOT S(0)) ) OR ((NOT S(3)) AND S(1) AND S(0))); 
	bin(2) <= (( S(2) and (not S(1))) or ( S(2) AND (NOT S(0)) ) OR ((NOT S(3)) AND S(1) AND S(0))); 
	bin(1) <= (( S(0) and (not S(1))) or ( S(1) AND (NOT S(0)) )); 
	bin(0) <= (NOT S(0));
	
	Out_bin <= bin;
	
	REG: registrador port map (clock, reset, S, enter, qprox);
	decoder: decod port map (bin, out_hex);
	
	

end circuito;
