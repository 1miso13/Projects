class FormClass{
	constructor(form){
		var _this = this;
		this.inputs = form.getElementsByTagName("input"); 
		form.oninput = function () {
			_this.FormValidate(_this.inputs);
		}
		this.FormValidate(this.inputs);
	}

	InputValidate(input){
		//ako prve sa testuje, ci je input prazdny. 
		//ak je prazdny, testuje sa iba na required parameter a ukonci sa
		if(input.value.length == 0)
		{
			if(input.hasAttribute("required")){
				input.className = "inputErr";
				return false;
			}
			else{
				input.className = "inputDef";
				return true;
			}
		}
		else{
		//postupne prechadzat vsetky moznosti a ak sa prejdu vsetky, nastavi sa inputOK,
		//ak sa v niektorom objavi chyba, nastavi sa inputErr
		if(input.hasAttribute("lenghtmax")){
			if(input.value.length > input.getAttribute("lenghtmax"))
			{
				input.className = "inputErr";
				return false;
			}
		}
		if(input.hasAttribute("lenghtmin")){
			if(input.value.length < input.getAttribute("lenghtmin"))
			{
				input.className = "inputErr";
				return false;
			}
		}
		if(input.hasAttribute("pattern")){
			if(!input.value.match(input.getAttribute("pattern")))
			{
				input.className = "inputErr";
				return false;
			}
		}
		
		input.className = "inputOK";
		return true;
		}
	}

	FormValidate(inputs){
		//postupne sa prechadzaju vsetky input vo form. Ak sa pri niektorom zisti inputErr, vypne sa odoslanie
		
		var ret = true;
		for(var i = 0; i<inputs.length; i++)
		{
			if (inputs[i].type.toLowerCase() == "text") {
				if(!this.InputValidate(inputs[i])){
					ret=false;
				}
			}
		}
		
		//submit
		for (var i = 0; i < inputs.length; i++) {
			if (inputs[i].type.toLowerCase() == "submit") {
				inputs[i].disabled = !ret;
			}
		}
		return ret;
	}

}

window.onload = function () {
	var forms = document.getElementsByTagName('form');
	for (var i = 0; i < forms.length; i++) {
		new FormClass(forms.item(i));
	};
	
}