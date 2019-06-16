<?
function printTable($result)
{
	$first_row = true;
		$id = 0;
		//$result = mysql_query($query,$db);
		$num_rows = mysql_num_rows($result);
		?>Pocet riadkov <? echo $num_rows; ?><br><br><?

		echo '<table ID="tabulkaPP" cellspacing=0>';
		while ($row = mysql_fetch_assoc($result)) {
		if ($first_row) {
			$first_row = false;
			// Output header row from keys.
			echo '<tr>';
			foreach($row as $key => $field) {
				echo '<th>' . htmlspecialchars($key) . '</th>';
			}
			echo '</tr>';
		}
		//$pokus = stripslashes(mysql_result($result,$row,0));
		$xx = 1;
		echo "<tr align=center>";
		//echo "<a href='?menu=0&'".$row[0].">";
		foreach($row as $key => $field) {
			if($xx == 1)
			{$xx = 2; $id = htmlspecialchars($field);}
			
			echo "<td><a display:inline-block;><div>" . htmlspecialchars($field) . "</div></a></td>";
			
		}
		//echo "</a>";
		echo '</tr>';
		}
		echo '</table>';
}
?>