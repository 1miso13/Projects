<?

function filter_mydate($s) 
{
    if (preg_match('@^(\d\d\d\d)-(\d\d)-(\d\d)$@', $s, $m) == false) 
	{
        return false;
    }
	else
	{
		if (checkdate($m[2], $m[3], $m[1]) == false)
		{		
			return false;
		}
		return true;
	}
	
	//return true;
 }

?>