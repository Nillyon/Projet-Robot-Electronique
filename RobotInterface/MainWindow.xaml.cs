using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ExtendedSerialPort;
using System.IO.Ports;


namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        SerialPort serialPort1;

        
        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("Com3", 115200, Parity.None, 8, StopBits.One);
            serialPort1.Open();
        }
        public void SendMessage()
        {
            textBoxReception.Text += "Reçu : " + textBoxEmission.Text + "\n";
            textBoxEmission.Text = "";
            serialPort1.WriteLine;
        }

        private void textBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void textBox_TextChanged_1(object sender, TextChangedEventArgs e)
        {

        }

        private void button_Click(object sender, RoutedEventArgs e)
        {

        }

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            if (buttonEnvoyer.Background == Brushes.RoyalBlue)
                buttonEnvoyer.Background = Brushes.Beige; 
            else
                buttonEnvoyer.Background = Brushes.RoyalBlue; //Quand on clique sur le bouton, il devient bleu et on utilise de l'UC

            SendMessage();

        }

        
        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if(e.Key == Key.Enter )
            {
                SendMessage();
            }

        }
    }
}
