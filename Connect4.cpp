
#include <iostream>
#include <SerialStream.h>
#include <ctime>
#include "IP.h"
#include "AI.h"


using namespace std ;
using namespace LibSerial;

bool arbit=false;

bool turn(bool diff, int temp, int comp, bool &flagcomp)
{
    if(temp==comp)
    {
      arbit = true;
      return true;

    }
    else 
    {
      if(diff)
      {
        if(arbit){flagcomp=false;arbit=false;}
        return false;
      }
      else
      {
        return true;
      }

    }
}

int main()
{
    SerialStream serial_port ;
    serial_port.Open("/dev/ttyACM0");
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_9600 );
    serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 );
    serial_port.SetParity( SerialStreamBuf::PARITY_EVEN );
    serial_port.SetVTime(10);
    serial_port.SetNumOfStopBits(1);
    tictactoe temp, Connect4State, prev;
	int cnt=0, R=0, Y=1;

    VideoCapture videoCam(0);         // using default camera
    int winner,i,j,ch,posy,flag,x,win=-1, CntRY=0;
    int flagDiff=0;
    bool flagcomp=0;
    time_t tCnt;
    char ard;
    

    Mat Connect4StateImage;//= imread("imageb.jpg" , CV_LOAD_IMAGE_COLOR);
    //Mat Connect4ChipsRY[2]= {Mat::zeros(Size(Connect4StateImage.rows, Connect4StateImage.cols), CV_8UC1), Mat::zeros(Size(Connect4StateImage.rows, Connect4StateImage.cols), CV_8UC1)}; //single channel
    Mat Connect4HSV_RY;
    Mat Connect4ChipsRY[3];

    vector <CartesianPoints>::iterator chipsPosIt;
    vector <CartesianPoints> allChipsCentres;


    cout<<"Step1"<<endl;
    if(!videoCam.isOpened())
    {
       cout<<"\nCan't access video cam!!!";
       return -1;
    }
    //namedWindow("Control", CV_WINDOW_AUTOSIZE);
   
    cout<<"Step2"<<endl;
    while(1)                                                               // continuous feed
    {
        //tCnt=clock();
        //    while((float)((clock()-tCnt)/CLOCKS_PER_SEC)<=0.05){;}


        bool isSuccess= videoCam.read(Connect4StateImage);
         //videoCam>>Connect4StateImage;
        if(!isSuccess)
        {
           cout<<"\nCannot read a frame from video!!!";
           continue;
        }
        //Hough
        



        //Hough



        resize(Connect4StateImage,Connect4StateImage,Size(595,510));
        cvtColor(Connect4StateImage, Connect4HSV_RY, COLOR_BGR2HSV);
        inRange(Connect4HSV_RY, Scalar(RED1L, REDSATL, VALL), Scalar(RED1H, SATH, VALH), Connect4ChipsRY[0]);
        inRange(Connect4HSV_RY, Scalar(YELLOWL, YELLOWSATL, VALL), Scalar(YELLOWH, SATH, VALH), Connect4ChipsRY[1]);
        inRange(Connect4HSV_RY, Scalar(RED2L, REDSATL, VALL), Scalar(RED2H, SATH, VALH), Connect4ChipsRY[2]);
        Connect4ChipsRY[0]=Connect4ChipsRY[0]+Connect4ChipsRY[2];
        //Mat Connect4ChipsRY[2]= {Mat::zeros(Size(Connect4StateImage.rows, Connect4StateImage.cols), CV_8UC1), Mat::zeros(Size(Connect4StateImage.rows, Connect4StateImage.cols), CV_8UC1)};
        //digitaliseImage(Connect4StateImage, Connect4ChipsRY);
            //cv::imshow("ImageWindow", Connect4StateImage);
        int erosion_size=5;
        int erosion_type=MORPH_ELLIPSE;
        Mat element= getStructuringElement(erosion_type, Size(2*erosion_size+1, 2*erosion_size+1), Point(erosion_size, erosion_size));
            //morphological opening (remove small objects from the foreground)
  erode(Connect4ChipsRY[0], Connect4ChipsRY[0], getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( Connect4ChipsRY[0], Connect4ChipsRY[0], getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

   //morphological closing (fill small holes in the foreground)
  dilate( Connect4ChipsRY[0], Connect4ChipsRY[0], getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  erode(Connect4ChipsRY[0], Connect4ChipsRY[0], getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological opening (remove small objects from the foreground)
  erode(Connect4ChipsRY[1], Connect4ChipsRY[1], getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( Connect4ChipsRY[1], Connect4ChipsRY[1], getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

   //morphological closing (fill small holes in the foreground)
  dilate( Connect4ChipsRY[1], Connect4ChipsRY[1], getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  erode(Connect4ChipsRY[1], Connect4ChipsRY[1], getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );





        for(int i=0; i<6; i++)
        {
            for(int j=0; j<7; j++)
            {
               temp.board[i][j]=-1;                              //initialise with 'E' i.e. empty state
            }
        }

        imshow("Red", Connect4ChipsRY[0]);
    imshow("Yellow", Connect4ChipsRY[1]); //show the thresholded image
  imshow("Original", Connect4StateImage);

      CntRY=0;

      if (waitKey(30) >= 0)
           break;
        //waitkey(2000);

        for(int i=0; i<2 ;i++)
        {
            //cv::imshow("ImageWindow", Connect4ChipsRY[i]);
            //waitKey(0);

            GaussianBlur(Connect4ChipsRY[i], Connect4ChipsRY[i], Size(9,9), 0,0);

            dilate( Connect4ChipsRY[i], Connect4ChipsRY[i], element);                 // to remove noise etc. in image data feed
            erode(Connect4ChipsRY[i], Connect4ChipsRY[i], element);

            edgeDetect( Connect4ChipsRY[i], Connect4ChipsRY[i]);

            //waitKey(0);
            findAllChipCentres(Connect4ChipsRY[i], allChipsCentres);

            for(chipsPosIt=allChipsCentres.begin(); chipsPosIt!=allChipsCentres.end(); chipsPosIt++ )
            {

               // cout<<"chipe centre ("<<chipsPosIt->y<<","<<chipsPosIt->x<<")"<<endl;
              temp.board[(int)(chipsPosIt->y/85)][(int)(chipsPosIt->x/85)]=((i==0)? R:Y);   
            // assigning 'R', 'Y' corresponding to the location of Red and Yellow chips
              CntRY++;  
            }
            allChipsCentres.clear();
             
            //cv::imshow("ImageWindow", Connect4ChipsRY[0]);
            }
            
         
        //temp.printBoard();
        
        if(!temp.checkIfValid(win)) 
          {

            for(int i=0; i<6; i++)
            {
                for(int j=0; j<7; j++)
                {
                  temp.board[i][j]=-1;                   
                }
                
            }
        
              continue;       // glitch in input
         }


      	
        
         //cout<<(clock()-t)/CLOCKS_PER_SEC<<endl;

        //  if((int)(float)((clock()-t)/CLOCKS_PER_SEC)<=5)
        // {
        //   continue;
        // }
        // t=clock();
         //cout<<"Step3\n";
        flagDiff=0;
        //cout<<((float)(t-clock())/CLOCKS_PER_SEC)<<endl;
        // if((int)(float)((clock()-t)/CLOCKS_PER_SEC)>=5)
        // {
        //     for(int i=0; i<6; i++)
        //     {
        //         for(int j=0; j<7; j++)
        //         {
        //            cout<<temp.board[i][j] ;
        //         }
        //         cout<<endl;
        //     }
        // }
         
        
        // if((int)(float)((clock()-t)/CLOCKS_PER_SEC)>=5)
        //  {
        //      temp.printBoard();
        //  }
        //     t=clock();
        //    cout<<endl<<endl;

            for(int i=0; i<6; i++)
            {
                for(int j=0; j<7; j++)
                {
                   if(Connect4State.board[i][j]!=temp.board[i][j])
                       {
                            flagDiff=1;  

                        }                  // check if move made
                }
                //cout<<endl;
            }
            //if(flagDiff==0&&flagcomp){flagcomp=0;}
            //cout<<flagDiff<<endl;
            //cout<<temp.NoOfChipsOnBoard()<<Connect4State.NoOfChipsOnBoard()<<flagDiff<<endl;
            if(turn(flagDiff, temp.NoOfChipsOnBoard(), Connect4State.NoOfChipsOnBoard(), flagcomp))
            {
              
                continue;
              }
            else
            { 
              
              if(flagcomp){continue;}
              
              for(int i=0; i<6; i++)
              {
                for(int j=0; j<7; j++)
                {
                  Connect4State.board[i][j]=temp.board[i][j];
                   //prev.board[i][j]=temp.board[i][j];          // assigning since move made
                }
              }
            
            
            }
            

            
		    //Connect4State.printBoard();
	     	//t.printHeuristic();
		    if(Connect4State.checkGameOver())
		    {     if(win==0)
		   	    cout<<"Game Over, You win :( \n";
			  else
                            cout<<"Game Over, I win :) :P :) \n";  
                          break;
		    }

		    Connect4State.nextBestMove();
       
 	//cout<<"("<<t.returnPosX()<<","<<t.returnPosY()<<")"<<endl;
		    cout<<"Col no :"<< Connect4State.returnPosY()+1<<endl;
        ard = '0' + (Connect4State.returnPosY()+1);
        serial_port<<ard;
		    Connect4State.printBoard();
        
		//t.printHeuristic();
		//posy=t.returnPosY();
		//serial_port<<posy;
		    if(Connect4State.checkGameOver())
	   	    {       if(win==0)
		  	      cout<<"Game Over, You win :(:P:( \n";
			    else
                              cout<<"Game Over, I win :) :P :) \n";
                            break;
		    }
        flagcomp=1;
              // Connect4State.board[Connect4State.posx][Connect4State.posy]=-1;
              // for(int i=0; i<6; i++)
              // {
              //   for(int j=0; j<7; j++)
              //   {
              //      prev.board[i][j]=Connect4State.board[i][j];          // assigning since move made
              //   }
              // }
              
        //cv::imshow("ImageWindow", Connect4ChipsRY[1]);
        if (waitKey(30) >= 0)
           break;
    }

	
	// t.nextBestMove();
	// t.printBoard();

	return 0;
}




