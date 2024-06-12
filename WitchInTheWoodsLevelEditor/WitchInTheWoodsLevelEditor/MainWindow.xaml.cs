using Microsoft.Win32;
using System.IO;
using System.Printing;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WitchInTheWoodsLevelEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public enum Tile
        {
            FREE = 'X',
            EDIFICE = 'E',
            WALL = 'W',
            TREE = 'T',
            ENEMY = 'S',
            PLAYER = '1'
        }

        string folderDirectory = string.Empty;
        Button[] button = new Button[14 * 18];
        Tile[] tiles = new Tile[14 * 18];

        Tile currentTile = Tile.FREE;

        public MainWindow()
        {
            this.ResizeMode = ResizeMode.NoResize;
            InitializeComponent();
            CreateLevelButtons();
            SelectFolderButton.Click += SelectFolderOnClick;
            ExportLevelButton.Click += ExportLevelOnClick;
            ImportLevelButton.Click += ImportLevelOnClick;

            FreeSpaceButton.Click += SelectFreeTileOnClick;
            TreeSpaceButton.Click += SelectTreeTileOnClick;
            EdificeSpaceButton.Click += SelectEdificeTileOnClick;
            WallSpaceButton.Click += SelectWallTileOnClick;
            EnemySpaceButton.Click += SelectEnemyTileOnClick;
            PlayerSpaceButton.Click += SelectPlayerTileOnClick;
        }
        public void CreateLevelButtons()
        {
            for (int i = 0; i < 14 * 18; i++)
            {
                button[i] = new Button();
                button[i].Name = "i" + i.ToString();
                button[i].Width = 20;
                button[i].Height = 20;
                button[i].Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/free.png", UriKind.Relative)) };
                button[i].Style = (Style)this.Resources["GridButton"];
                Canvas.SetLeft(button[i], (i % 14) * 20);
                Canvas.SetTop(button[i], (i / 14) * 20);
                LevelCanvas.Children.Add(button[i]);
                tiles[i] = Tile.FREE;
                button[i].Click += TileButtonsOnClick;
            }
        }
        public void TileButtonsOnClick(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;

            int index = Int32.Parse(Regex.Match(btn.Name, @"\d+").Value);
            switch(currentTile)
            {
                case Tile.FREE:
                    btn.Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/free.png", UriKind.Relative)) };
                    tiles[index] = Tile.FREE;
                    break;
                case Tile.TREE:
                    btn.Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/tree.png", UriKind.Relative)) };
                    tiles[index] = Tile.TREE;
                    break;
                case Tile.EDIFICE:
                    btn.Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/edifice.png", UriKind.Relative)) };
                    tiles[index] = Tile.EDIFICE;
                    break;
                case Tile.WALL:
                    btn.Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/wall.png", UriKind.Relative)) };
                    tiles[index] = Tile.WALL;
                    break;
                case Tile.ENEMY:
                    btn.Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/enemy.png", UriKind.Relative)) };
                    tiles[index] = Tile.ENEMY;
                    break;
                case Tile.PLAYER:
                    btn.Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/player1.png", UriKind.Relative)) };
                    tiles[index] = Tile.PLAYER;
                    break;
            }       
        }
        public void SelectFolderOnClick(object sender, RoutedEventArgs e)
        {
            OpenFolderDialog dialog = new OpenFolderDialog()
            {
                Title = "Please select 'stage' folder in WitchInTheWoods project."
            };

            if (dialog.ShowDialog() == true)
            {
                folderDirectory = dialog.FolderName;
                SelectFolderDisplay.Text = folderDirectory;
            };
        }
        public void ExportLevelOnClick(object sender, RoutedEventArgs e)
        {
            int enemiesCount = tiles.Count(s => s == Tile.ENEMY);
            int playerCount = tiles.Count(s => s == Tile.PLAYER);

            if (enemiesCount != 8 || playerCount != 1 || folderDirectory == String.Empty)
            {
                string notice = "The following error occured:";
                if (enemiesCount != 8)
                {
                    notice += "\n- Enemy Spawn Point must have 8 positions.";
                }
                if (playerCount != 1)
                {
                    notice += "\n- Player Spawn Point must have 1 position.";
                }
                if (folderDirectory == String.Empty)
                {
                    notice += "\n- Please select 'stage' folder in 'WitchInTheWoods' project.";
                }
                MessageBox.Show(notice);
            }
            else
            {
                try
                {
                    StreamWriter sw = new StreamWriter(folderDirectory + "\\stage_1.csv");
                    for (int i = 0; i < tiles.Count(); i++)
                    {
                        if (i % 14 != 13)
                        {
                            sw.Write(((char)tiles[i]).ToString() + ",");
                        }
                        else
                        {
                            sw.WriteLine(((char)tiles[i]).ToString());
                        }
                    }
                    sw.Close();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Exception: " + ex.Message);
                }
                finally
                {
                    MessageBox.Show("Level Export Completed");
                }

            }
        }
        public void ImportLevelOnClick(object sender, RoutedEventArgs e)
        {
            String dir = String.Empty;
            String readLine = String.Empty;
            String[] values = { String.Empty };

            OpenFileDialog dialog = new OpenFileDialog()
            {
                Title = "Please select 'stage_1.csv' file in 'stage' folder in WitchInTheWoods project."
            };

            if (dialog.ShowDialog() == true)
            {
                dir = dialog.FileName;

                try
                {
                    StreamReader sr = new StreamReader(dir);
                    for (int y = 0; y < 18; y++)
                    {
                        readLine = sr.ReadLine();
                        values = readLine.Split(",".ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                        for (int x = 0; x < 14; x++)
                        {
                            switch (values[x])
                            {
                                case "X":
                                    button[14 * y + x].Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/free.png", UriKind.Relative)) };
                                    tiles[14 * y + x] = Tile.FREE;
                                    break;
                                case "E":
                                    button[14 * y + x].Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/edifice.png", UriKind.Relative)) };
                                    tiles[14 * y + x] = Tile.EDIFICE;
                                    break;
                                case "W":
                                    button[14 * y + x].Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/wall.png", UriKind.Relative)) };
                                    tiles[14 * y + x] = Tile.WALL;
                                    break;
                                case "T":
                                    button[14 * y + x].Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/tree.png", UriKind.Relative)) };
                                    tiles[14 * y + x] = Tile.TREE;
                                    break;
                                case "S":
                                    button[14 * y + x].Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/enemy.png", UriKind.Relative)) };
                                    tiles[14 * y + x] = Tile.ENEMY;
                                    break;
                                case "1":
                                    button[14 * y + x].Background = new ImageBrush { ImageSource = BitmapFrame.Create(new Uri("picture/player1.png", UriKind.Relative)) };
                                    tiles[14 * y + x] = Tile.PLAYER;
                                    break;
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Exception: " + ex.Message);
                }
                finally
                {
                    MessageBox.Show("Level Import Completed");
                }
            }
        }
        public void SelectFreeTileOnClick(object sender, RoutedEventArgs e)
        {
            SelectCurrentTile(Tile.FREE);
        }
        public void SelectTreeTileOnClick(object sender, RoutedEventArgs e)
        {
            SelectCurrentTile(Tile.TREE);
        }
        public void SelectEdificeTileOnClick(object sender, RoutedEventArgs e)
        {
            SelectCurrentTile(Tile.EDIFICE);
        }
        public void SelectWallTileOnClick(object sender, RoutedEventArgs e)
        {
            SelectCurrentTile(Tile.WALL);
        }
        public void SelectEnemyTileOnClick(object sender, RoutedEventArgs e)
        {
            SelectCurrentTile(Tile.ENEMY);
        }
        public void SelectPlayerTileOnClick(object sender, RoutedEventArgs e)
        {
            SelectCurrentTile(Tile.PLAYER);
        }
        public void SelectCurrentTile(Tile tile)
        {
            switch (currentTile)
            {
                case Tile.FREE:
                    FreeSpaceDisplay.Foreground = new SolidColorBrush(Color.FromArgb(255, 0, 0, 0));
                    break;
                case Tile.TREE:
                    TreeSpaceDisplay.Foreground = new SolidColorBrush(Color.FromArgb(255, 0, 0, 0));
                    break;
                case Tile.EDIFICE:
                    EdificeSpaceDisplay.Foreground = new SolidColorBrush(Color.FromArgb(255, 0, 0, 0));
                    break;
                case Tile.WALL:
                    WallSpaceDisplay.Foreground = new SolidColorBrush(Color.FromArgb(255, 0, 0, 0));
                    break;
                case Tile.ENEMY:
                    EnemySpaceDisplay.Foreground = new SolidColorBrush(Color.FromArgb(255, 0, 0, 0));
                    break;
                case Tile.PLAYER:
                    PlayerSpaceDisplay.Foreground = new SolidColorBrush(Color.FromArgb(255, 0, 0, 0));
                    break;
            }

            switch (tile)
            {
                case Tile.FREE:
                    FreeSpaceDisplay.Foreground = new SolidColorBrush(Color.FromArgb(255, 255, 0, 0));
                    break;
                case Tile.TREE:
                    TreeSpaceDisplay.Foreground = new SolidColorBrush(Color.FromArgb(255, 255, 0, 0));
                    break;
                case Tile.EDIFICE:
                    EdificeSpaceDisplay.Foreground = new SolidColorBrush(Color.FromArgb(255, 255, 0, 0));
                    break;
                case Tile.WALL:
                    WallSpaceDisplay.Foreground = new SolidColorBrush(Color.FromArgb(255, 255, 0, 0));
                    break;
                case Tile.ENEMY:
                    EnemySpaceDisplay.Foreground = new SolidColorBrush(Color.FromArgb(255, 255, 0, 0));
                    break;
                case Tile.PLAYER:
                    PlayerSpaceDisplay.Foreground = new SolidColorBrush(Color.FromArgb(255, 255, 0, 0));
                    break;
            }

            currentTile = tile;
        }
    }
}