/******************************************************************/
/** blind search algorithms by maskotky                          **/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <deque>

#define INF 99999999
#define DFS_ALGORITHM 1
#define BFS_ALGORITHM 2
#define DIRECTIONS_FOUR 4
#define DIRECTIONS_EIGHT 8
#define CONSOLE
#define FILE
//#define DEBUGCONSOLE

using namespace std;

class Point
{
  public:
         int x;
         int y;
  Point(){x=y=0;}
  bool operator==(const Point &p)
  {
      if((this->x == p.x) && (this->y == p.y))
        return true;
      else
        return false;
  }
  Point &operator=(Point & p2)
  {
    if(&p2 != this)
    {
        x = p2.x;
        y = p2.y;
    }
    return *this;
  }
};

class NodeObject
{
      public:
             int no; //number4fields
             int value;
             int key;
             int tick; //was=1, was not=0
             int path; //path's length
      NodeObject(){no=value=tick=0;key=path=INF;}
};

class Robot
{
  private:
     int k;
     int i;
     int j;
     NodeObject **Tab; // boards tab
     int **TabNeighbor; // neighbors' tab
     int movement_mode; //max nodes
     int node;
     int row,col,n;
     Point *Dict; //
     Point start, meta;
     Point position;
     int startNode;
     int SearchStartNode();
     int DFS(int);
     int BFS(int);
     deque <int> StackN;
     deque <int> QueueN;
 public:
     Robot();
     void ReadMap(const char *fileName);
     void ReadTask(Point, Point);
     void PrintBvalues();
     void PrintMaze();
     void PrintKeys();
     void PrintDict();
     void PrintTabNeighbor();
     void CreateGraph(int);
     void PlanPath(int);
     Point GetRobotPosition();

};

Robot::Robot(){}

void Robot::ReadMap(const char *fileName)
{
     ifstream file;
     file.open(fileName,std::ios::in);

     if(file.good()==true)
     {
       #ifdef CONSOLE
       cout<<"input data file opend!"<<endl;
       #endif
       file>>row>>col;
       if(row <= 0 || col <= 0)
       {
         #ifdef CONSOLE
         cout<<"wrong input data, row/col values error!!"<<endl;
         #endif
         getchar(); exit(-1);
       }

       Tab=new NodeObject *[row];
       for (int i=0;i<row;i++)
            Tab[i]=new NodeObject[col];

       for(i = 0; i < row; i++)
       {
        for(j = 0; j < col; j++)
        {
         file>>Tab[i][j].value;
          if(Tab[i][j].value != 1 && Tab[i][j].value != 0)
          {
            #ifdef CONSOLE
            cout<<"wrong intput data, scene representation error!!"<<endl;
            #endif
            getchar(); exit(-1);
          }
        }
       }

     file.close();
     }
     else
     {
       #ifdef CONSOLE
       cout<<"can't open the file!!!"<<endl;
       #endif
       getchar();exit(-1);
     }
}

void Robot::ReadTask(Point S, Point M)
{
    start = S;
    meta = M;
    #ifdef CONSOLE
    cout<<"start (x,y) = ["<<start.x<<"]["<<start.y<<"]"<<endl;
    cout<<"goal (x,y) = ["<<meta.x<<"]["<<meta.y<<"]"<<endl;
    #endif
}

void Robot::CreateGraph(int mode)
{
      k=0;
      movement_mode = mode;
      for(int i=0;i<row;i++)
      {
       for(int j=0;j<col;j++)
        {
         if(Tab[i][j].value==0)
          {
           Tab[i][j].key=k;
           k++;
          }
        }
       }

      n=k;
      Dict=new Point[k];
      int index=0;
      for(int i=0;i<row;i++)
       {
        for(int j=0;j<col;j++)
         {
          if(Tab[i][j].value==0)
           {
            Dict[index].x=i;
            Dict[index].y=j;
            index++;
           }
        }
      }

        TabNeighbor=new int*[n];
        if(mode == 4)
        {
            #ifdef CONSOLE
            cout<<"4 directions movement!"<<endl;
            #endif
            for(int i=0;i<n;i++)
                TabNeighbor[i]=new int[4];
        }
        if(mode == 8)
        {
            #ifdef CONSOLE
            cout<<"8 directions movement!"<<endl;
            #endif
            for(int i=0;i<n;i++)
                TabNeighbor[i]=new int[8];
        }

    for(k = 0; k < n; k++)
    {
            if(Tab[Dict[k].x+1][Dict[k].y].value==0)
            {TabNeighbor[k][0]=Tab[Dict[k].x+1][Dict[k].y].key;}
            else{TabNeighbor[k][0]=-1;}

            if(Tab[Dict[k].x-1][Dict[k].y].value==0)
            {TabNeighbor[k][1]=Tab[Dict[k].x-1][Dict[k].y].key;}
            else{TabNeighbor[k][1]=-1;}

            if(Tab[Dict[k].x][Dict[k].y+1].value==0)
            {TabNeighbor[k][2]=Tab[Dict[k].x][Dict[k].y+1].key;}
                      else{TabNeighbor[k][2]=-1;}

            if(Tab[Dict[k].x][Dict[k].y-1].value==0)
            {TabNeighbor[k][3]=Tab[Dict[k].x][Dict[k].y-1].key;}
            else{TabNeighbor[k][3]=-1;}

    if(mode == 8)
    {
            if(Tab[Dict[k].x+1][Dict[k].y+1].value==0)
            {TabNeighbor[k][4]=Tab[Dict[k].x+1][Dict[k].y+1].key;}
            else{TabNeighbor[k][4]=-1;}

            if(Tab[Dict[k].x+1][Dict[k].y-1].value==0)
            {TabNeighbor[k][5]=Tab[Dict[k].x+1][Dict[k].y-1].key;}
            else{TabNeighbor[k][5]=-1;}

            if(Tab[Dict[k].x-1][Dict[k].y-1].value==0)
            {TabNeighbor[k][6]=Tab[Dict[k].x-1][Dict[k].y-1].key;}
            else{TabNeighbor[k][6]=-1;}

            if(Tab[Dict[k].x-1][Dict[k].y+1].value==0)
            {TabNeighbor[k][7]=Tab[Dict[k].x-1][Dict[k].y+1].key;}
            else{TabNeighbor[k][7]=-1;}
    }
  }
}

void Robot::PrintBvalues()
{
  cout<<"dim = "<<row<<" x "<<col<<endl<<endl;
  cout<<"BOARD: "<<endl;
    for(int i=0;i<row;i++)
    {
      for(int j=0;j<col;j++)
      {
        cout<<Tab[i][j].value<<" ";
      }
      cout<<" "<<endl;
    }
}

void Robot::PrintMaze()
{
    cout<<"scene:"<<endl;
  for(int i=0;i<row;i++)
  {
    for(int j=0;j<col;j++)
    {
        if(Tab[i][j].value==1){cout<<char(-79);}
        else cout<<" ";
    }
    cout<<" "<<endl;
  }
}

void Robot::PrintKeys()
{
  for(int i=0;i<row;i++)
  {
    for(int j=0;j<col;j++)
    {
      if(Tab[i][j].key==INF){cout<<char(-79);}
      else cout<<Tab[i][j].key;
    }
    cout<<" "<<endl;
  }
}

void Robot::PrintDict()
{
    cout<<"ilosc kluczy:"<<k<<endl;
    for(int i=0;i<k;i++)
    cout<<Dict[i].x<<", "<<Dict[i].y<<endl;
}

void Robot::PrintTabNeighbor()
{
    for(int i=0;i<n;i++)
    {
    cout<<i<<"-";
    for(int j=0;j<movement_mode;j++)
    {
        if(TabNeighbor[i][j]==-1){cout<<"x ";}
        else{cout<<TabNeighbor[i][j]<<" ";}
    }
    cout<<" "<<endl;
    }
}

int Robot::SearchStartNode()
{
    return Tab[start.y][start.x].key;
}

void Robot::PlanPath(int algorithm)
{
    int node = 0;
    startNode = SearchStartNode();

    if(algorithm == DFS_ALGORITHM)
    {
        #ifdef CONSOLE
        cout<<"Depth First Search logs:"<<endl;
        cout<<"start node: "<<startNode<<endl<<endl;
        #endif
        StackN.push_front(startNode);
        node = DFS(startNode);
        #ifdef CONSOLE
        cout<<node<<" nodes processed"<<endl;
        #endif
    }
    if(algorithm == BFS_ALGORITHM)
    {
        #ifdef CONSOLE
        cout<<"Breadth First Search logs:"<<endl;
        cout<<"start node: "<<startNode<<endl<<endl;
        #endif
        QueueN.push_back(startNode);
        node = BFS(startNode);
        #ifdef CONSOLE
        cout<<node<<" nodes processed"<<endl;
        #endif
    }

}

int Robot::DFS(int node)
{
    int processedNodes = 0;
    int v;
    int horizontal;
    int vertical;
    bool fileFlag = false;
    #ifdef FILE
    fstream file;
    file.open("data_out.txt",std::ios::out);
    if(file.good() == true)
    {
        fileFlag = true;
    }
    #endif // FILE

    while(!StackN.empty())
    {
        if(position == meta)
        {
            #ifdef CONSOLE
            cout<<"*---------------------------------------------*"<<endl;
            cout<<"*                                             *"<<endl;
            cout<<"goal!!"<<endl;
            cout<<processedNodes<<" processed nodes 2 reach the goal!!"<<endl;
            cout<<"*                                             *"<<endl;
            cout<<"*---------------------------------------------*"<<endl;
            #endif
        }

        v = StackN[0];
        StackN.pop_front();
        #ifdef CONSOLE
        cout<<"operating node: "<<v<<endl;
        #endif

        if(Tab[Dict[v].x][Dict[v].y].tick != 1)
        {
          Tab[Dict[v].x][Dict[v].y].tick = 1;
          position = Dict[v];
          processedNodes++;
          #ifdef CONSOLE
          cout<<"(row,col) = ("<<position.x<<","<<position.y<<")"<<endl;
          cout<<"neighbor nodes 2 visit: ";
          #endif
          #ifdef FILE
          if(fileFlag) file<<position.x<<" "<<position.y<<endl;
          #endif // FILE
          for(i = 0; i < movement_mode; i++)
          {
              if(TabNeighbor[v][i] != -1)
              {
                  horizontal = Dict[TabNeighbor[v][i]].x;
                  vertical = Dict[TabNeighbor[v][i]].y;
                  if(Tab[horizontal][vertical].tick == 0)
                  {
                      #ifdef CONSOLE
                      cout<<Tab[horizontal][vertical].key<<"=("<<horizontal<<","<<vertical<<") ";
                      #endif
                      StackN.push_front(Tab[horizontal][vertical].key);
                  }
              }
          }
          #ifdef FILE
          if(fileFlag)
          {
              for(i = 0; i < StackN.size(); i++)
              {
                  file<<Dict[StackN[i]].x<<" "<<Dict[StackN[i]].y<<" ";
              }
              file<<endl;
          }
          #endif // FILE
        }
        #ifdef CONSOLE
        cout<<endl;
        cout<<"stack size: "<<StackN.size()<<endl;
        cout<<"-----------------------------------------------"<<endl;
        #endif

    }
    #ifdef FILE
    if(fileFlag)file.close();
    #endif // FILE
    return processedNodes;
}

int Robot::BFS(int node)
{
    int processedNodes = 0;
    int v;
    int horizontal;
    int vertical;
    bool fileFlag = false;
    #ifdef FILE
    fstream file;
    file.open("data_out.txt",std::ios::out);
    if(file.good() == true)
    {
        fileFlag = true;
    }
    #endif // FILE

    while(!QueueN.empty())
    {
        if(position == meta)
        {
            #ifdef CONSOLE
            cout<<"*---------------------------------------------*"<<endl;
            cout<<"*                                             *"<<endl;
            cout<<"goal!!"<<endl;
            cout<<processedNodes<<" processed nodes 2 reach the goal!!"<<endl;
            cout<<"*                                             *"<<endl;
            cout<<"*---------------------------------------------*"<<endl;
            #endif
        }

        v = QueueN[0];
        QueueN.pop_front();
        #ifdef CONSOLE
        cout<<"operating node: "<<v<<endl;
        #endif

        if(Tab[Dict[v].x][Dict[v].y].tick != 1)
        {
          Tab[Dict[v].x][Dict[v].y].tick = 1;
          position = Dict[v];
          processedNodes++;
          #ifdef CONSOLE
          cout<<"(row,col) = ("<<position.x<<","<<position.y<<")"<<endl;
          cout<<"neighbor nodes 2 visit: ";
          #endif
          #ifdef FILE
          if(fileFlag) file<<position.x<<" "<<position.y<<endl;
          #endif // FILE
          for(i = 0; i < movement_mode; i++)
          {
              if(TabNeighbor[v][i] != -1)
              {
                  horizontal = Dict[TabNeighbor[v][i]].x;
                  vertical = Dict[TabNeighbor[v][i]].y;
                  if(Tab[horizontal][vertical].tick == 0)
                  {
                      #ifdef CONSOLE
                      cout<<Tab[horizontal][vertical].key<<"=("<<horizontal<<","<<vertical<<") ";
                      #endif
                      QueueN.push_back(Tab[horizontal][vertical].key);
                  }
              }
          }
        }
        #ifdef FILE
          if(fileFlag)
          {
              for(i = 0; i < QueueN.size(); i++)
              {
                  file<<Dict[QueueN[i]].x<<" "<<Dict[QueueN[i]].y<<" ";
              }
              file<<endl;
          }
        #endif // FILE
        #ifdef CONSOLE
        cout<<endl;
        cout<<"queue size: "<<QueueN.size()<<endl;
        cout<<"-----------------------------------------------"<<endl;
        #endif
    }
    #ifdef FILE
    if(fileFlag)file.close();
    #endif // FILE
    return processedNodes;
}


int main()
{
    /********************************************/
    /** tutaj uzytkownik wprowadza dane        **/
    //string str = "scene/maze007.txt";
    string str = "scene/zera_10x10.txt";
    const char *dataFile = str.c_str();
    int modeMovementDirection = DIRECTIONS_FOUR;
    int modeSearchAlgorithm = DFS_ALGORITHM;
    Point Start;
    Point Meta;
    Start.x = 1;
    Start.y = 1;
    Meta.x = 8;
    Meta.y = 8;
    /********************************************/


    Robot MobileRobot;
    MobileRobot.ReadMap(dataFile);
    MobileRobot.ReadTask(Start, Meta);
    MobileRobot.CreateGraph(modeMovementDirection);

    #ifdef DEBUGCONSOLE
    MobileRobot.PrintBvalues();
    MobileRobot.PrintMaze();
    MobileRobot.PrintKeys();
    MobileRobot.PrintTabNeighbor();
    #endif

    MobileRobot.PlanPath(modeSearchAlgorithm);

    #ifdef CONSOLE
    cout<<" "<<endl;
    cout<<"______________"<<endl;
    cout<<"| maskotky's |"<<endl;
    cout<<"~~~~~~~~~~~~~~"<<endl;
    #endif
    getchar();
}
