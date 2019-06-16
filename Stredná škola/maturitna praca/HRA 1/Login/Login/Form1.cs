using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
namespace Login
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        public string username,password;
        private void Login_Click(object sender, EventArgs e)
        {
            username = user.Text ;
            password = Password.Text;
            //Application.Exit();
            if (username == "admin" && password == "admin")
            {
                for (int i = 0; i < 100; i++)
                {
                    progressBar1.Value = i;
                }
                //Application.Run(new Connecting_Failed());
                Process.Start("C:/Users/Acer/Desktop/tvorba 3D/tvorba 3D/tvorba 3D/bin/x86/Debug/tvorba 3D.exe");


            }
            else
            {
                /*Logging_Failed form = new Logging_Failed();
                
                form.ShowDialog();//this.Hide();*/
            }
        }

        
    }
}
