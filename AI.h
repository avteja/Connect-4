#include<iostream>
#include<cmath>
//#include <SerialStream.h>
#define M 6
#define N 7
#define K 4
#define D 8
//M = no. of rows
//N = no. of cols
//K = ~
using namespace std;
//using namespace LibSerial;
struct tictactoe
{
	int board[M][N];// 0 = player1 ; 1 = player2 ; -1 = empty square
	int cnt;// counts number of nodes explored
	int posx,posy;
	//int R, Y;
	bool isGameOver;
	tictactoe()
	{   posx=posy=-1;
		int i,j;

		for(i=0;i<M;++i)
		{
			for(j=0;j<N;++j)
			{
				board[i][j]=-1;
			}
		}
	}
	bool makeMove(int&,int);
	bool checkIfValid(int &winner);// Just checks if no. of Xs and Os are legitimate; Also checks if there are
	// multiple winning configs; returns true if valid
	int checkWinPos();// To be called only if position is valid
	// Checks if position is winning for any player; returns 0 if player1 has won, 1 if player2 has won, -1 otherwise
	int evaluate();// Evaluation Function - returns evaluation value
	int nextTurn();// Returns whose turn is next
	bool nextBestMove();// Prints board after making next best move
	void printBoard();// Prints current position of board
	int value(int,int,int);
	int max_value(int,int,int);
	int min_value(int,int,int);
	bool checkGameOver();
	void undoMove(int,int);
	int returnPosY();
	int returnPosX();
	void printHeuristic();
	int NoOfChipsOnBoard();
	// void setRY(int r, int y)
	// {
	//     R=r;
	//     Y=y;
	// }
};
int tictactoe::NoOfChipsOnBoard()
{	int cnt=0;
	for(int j=0;j<N;++j)
	{   
		for(int i=0;i<M;++i)
		{
			if(board[i][j]!=-1)
			{
				cnt++;
			}
			
		}
	}
	return cnt;
}
void tictactoe::printHeuristic()
{
	cout<<"Heuristic Value = "<<evaluate()<<endl;
}
int tictactoe::returnPosY()
{
	return posy;
}
int tictactoe::returnPosX()
{
	return posx;
}
void tictactoe::undoMove(int x,int y)
{
	board[posx][posy]=-1;
	board[x][y-1]=-1;
}
bool tictactoe::makeMove(int &x,int y)
{
    y--;
    x=-1;
    int t;
    t=nextTurn();
    if(y<N && (t==0 || t==1))
	{
	    for(x=M-1;x>-1;--x)
        {
            if(board[x][y]==-1)
            {
           		break;
            }
        }
        if(x==-1)
        {
      		return false;
        }
  		board[x][y]=t;
		return true;
	}
	return false;
}
bool tictactoe::checkIfValid(int &winner)
{
	winner=-1;
	int cnt1=0,cnt0=0,i,j,k,cons0=0,cons1=0,win0=0,win1=0, col0=0, col1=0 ;
	for(j=0;j<N;++j)
	{   col0=col1=0;
		for(i=0;i<M;++i)
		{
			if(board[i][j]==0)
			{
			    ++cnt0;
			    ++col0;
			}
			else if(board[i][j]==1)
			{
			    ++cnt1;
			    ++col1;
			}
			else if(board[i][j]==-1)
                        {
                           if(col0!=0||col1!=0)
                             return false;
                        }
		}
	}

	if(!(cnt1==cnt0 || cnt1==(cnt0-1)))
		return false;
	//Checking winning config in rows
	for(i=0;i<M;++i)
	{
		cons0=cons1=0;
		for(j=0;j<N;++j)
		{
			if(board[i][j]==0)
			{
				++cons0;
				cons1=0;
			}
			else if(board[i][j]==1)
			{
				++cons1;
				cons0=0;
			}
			else if(board[i][j]==-1)
			{
				cons0=cons1=0;
			}
			if(cons0>=K)
			{
				++win0;
			}
			else if(cons1>=K)
			{
				++win1;
			}
		}
	}
	//Checking winning config in cols
	for(j=0;j<N;++j)
	{
		cons0=cons1=0;
		for(i=0;i<M;++i)
		{
			if(board[i][j]==0)
			{
				++cons0;
				cons1=0;
			}
			else if(board[i][j]==1)
			{
				++cons1;
				cons0=0;
			}
			else if(board[i][j]==-1)
			{
				cons0=cons1=0;
			}
			if(cons0==K)
			{
				++win0;
			}
			else if(cons1==K)
			{
				++win1;
			}
		}
	}
	//Checking winning config in diags - part1(downwards)
	for(i=0;i<=M-K;++i)
	{
		for(j=0;j<=N-K;++j)
		{
			cons0=cons1=0;
			for(k=0;k<K;++k)
			{
				if(board[i+k][j+k]==0)
				{
					++cons0;
					cons1=0;
				}
				else if(board[i+k][j+k]==1)
				{
					++cons1;
					cons0=0;
				}
				else if(board[i+k][j+k]==-1)
				{
					cons0=cons1=0;
				}
				if(cons0==K)
				{
					++win0;
				}
				else if(cons1==K)
				{
					++win1;
				}
			}
		}
	}
	//Checking winning config in diags - part2(upwards)
	for(i=M-1;i>=K-1;--i)
	{
		for(j=0;j<=N-K;++j)
		{
			cons0=cons1=0;
			for(k=0;k<K;++k)
			{
				if(board[i-k][j+k]==0)
				{
					++cons0;
					cons1=0;
				}
				else if(board[i-k][j+k]==1)
				{
					++cons1;
					cons0=0;
				}
				else if(board[i-k][j+k]==-1)
				{
					cons0=cons1=0;
				}
				if(cons0==K)
				{
					++win0;
				}
				else if(cons1==K)
				{
					++win1;
				}
			}
		}
	}
	if(win0>=1 && win1>=1)
		return false;
	else
	{
		if(win0)
			winner=0;
		else if(win1)
			winner=1;
		return true;
	}
}
int tictactoe::checkWinPos()
{
	int i,j,winner;
	checkIfValid(winner);
	return winner;
}
void tictactoe::printBoard()
{
	int i,j;
	for(j=0;j<N;++j)
		cout<<"----";
	cout<<"-"<<endl;
	for(i=0;i<M;++i)
	{
	    cout<<"|";
		for(j=0;j<N;++j)
		{
			if(board[i][j]==0)
				cout<<" 0 |";
			else if(board[i][j]==1)
				cout<<" 1 |";
			else
				cout<<"   |";
		}
		cout<<endl;
		for(j=0;j<N;++j)
			cout<<"----";
		cout<<"-"<<endl;
	}
}
int tictactoe::nextTurn()
{
	int cnt1=0,cnt0=0,i,j;
	for(i=0;i<M;++i)
	{
		for(j=0;j<N;++j)
		{
			if(board[i][j]==0)
				++cnt0;
			else if(board[i][j]==1)
				++cnt1;
		}
	}
	if(checkGameOver())
		return -1;
	if(cnt0==cnt1)//&&R==0)||((cnt1==cnt0+1)&&R==1))
		return 0;
	else if(cnt0==cnt1+1)//&&R==0)||(cnt0==cnt1)&&R==1)
		return 1;
	//else
     //   return -1;
}
bool tictactoe::nextBestMove()
{
	int t=nextTurn();
	if(t==-1)
		return false;
	int i,j,posi=-1,posj=-1,m;
	cnt=0;
	value(0,-10000000,10000000);
	if(t==0)
		board[posx][posy]=0;
	else if(t==1)
		board[posx][posy]=1;
	return true;
}
int tictactoe::evaluate()
{
	int i,j,k,l,m,flag,kdash,cnt0,cnt1,cntempty,evalvalue=0,conscount1,conscount0;
	// Rows
	for(i=0;i<M;++i)
	{
		cnt0=cnt1=cntempty=0;
		for(j=0;j<N;++j)
		{
			if(board[i][j]==-1)
			{
				++cntempty;
			}
			else if(board[i][j]==0)
			{
				++cnt0;
				if(cnt1)
				{
					if(cntempty+cnt1>=K)
					{
						if(cnt1>=K)
						{
							conscount1=conscount0=0;
							for(l=0;l<j;++l)
							{
								if(board[i][l]==1)
								{
									++conscount1;
								}
								else
								{
									conscount1=0;
								}
								if(conscount1>=K)
								{
									evalvalue-=1000000;
									break;
								}
							}
						}
						evalvalue-=pow(K,cnt1);
					}
					cnt1=0;
					cntempty=0;
					if(board[i-1][j]==-1)
					{
						for(k=i-1;board[k][j]!=-1;--k)
						{
							++cntempty;
							if(k==-1)
								break;
						}
					}
				}
			}
			else if(board[i][j]==1)
			{
				++cnt1;
				if(cnt0)
				{
					if(cntempty+cnt0>=K)
					{
						if(cnt0>=K)
						{
							conscount1=conscount0=0;
							for(l=0;l<j;++l)
							{
								if(board[i][l]==0)
								{
									++conscount0;
								}
								else
								{
									conscount0=0;
								}
								if(conscount0>=K)
								{
									evalvalue+=1000000;
									break;
								}
							}
						}
						evalvalue+=pow(K,cnt0);
					}
					cnt0=0;
					cntempty=0;
					if(board[i-1][j]==-1)
					{
						for(k=i-1;board[k][j]==-1;--k)
						{
							++cntempty;
							if(k==-1)
								break;
						}
					}
				}
			}
		}
		if(cnt0)
		{
			if(cntempty+cnt0>=K)
					{
						if(cnt0>=K)
						{
							conscount1=conscount0=0;
							for(l=0;l<j;++l)
							{
								if(board[i][l]==0)
								{
									++conscount0;
								}
								else
								{
									conscount0=0;
								}
								if(conscount0>=K)
								{
									evalvalue+=1000000;
									break;
								}
							}
						}
						evalvalue+=pow(K,cnt0);
					}
		}
		else if(cnt1)
		{
			if(cntempty+cnt1>=K)
					{
						if(cnt1>=K)
						{
							conscount1=conscount0=0;
							for(l=0;l<j;++l)
							{
								if(board[i][l]==1)
								{
									++conscount1;
								}
								else
								{
									conscount1=0;
								}
								if(conscount1>=K)
								{
									evalvalue-=1000000;
									break;
								}
							}
						}
						evalvalue-=pow(K,cnt1);
					}
		}
	}
	// Cols
	//cout<<evalvalue<<endl;
	for(j=0;j<N;++j)
	{
		cnt0=cnt1=cntempty=0;
		for(i=0;i<M;++i)
		{
			if(board[i][j]==-1)
			{
				++cntempty;
			}
			else if(board[i][j]==0)
			{
				++cnt0;
				if(cnt1)
				{
					if(cntempty+cnt1>=K)
					{
						if(cnt1>=K)
						{
							conscount1=conscount0=0;
							for(l=0;l<i;++l)
							{
								if(board[l][j]==1)
								{
									++conscount1;
								}
								else
								{
									conscount1=0;
								}
								if(conscount1>=K)
								{
									evalvalue-=1000000;
									break;
								}
							}
						}
						evalvalue-=pow(K,cnt1);
					}
					cnt1=0;
					cntempty=0;
					if(board[i][j-1]==-1)
					{
						for(k=j-1;board[i][k]!=-1;--k)
						{
							++cntempty;
							if(k==-1)
								break;
						}
					}
				}
			}
			else if(board[i][j]==1)
			{
				++cnt1;
				if(cnt0)
				{
					if(cntempty+cnt0>=K)
					{
						if(cnt0>=K)
						{
							conscount1=conscount0=0;
							for(l=0;l<i;++l)
							{
								if(board[l][j]==0)
								{
									++conscount0;
								}
								else
								{
									conscount0=0;
								}
								if(conscount0>=K)
								{
									evalvalue+=1000000;
									break;
								}
							}
						}
						evalvalue+=pow(K,cnt0);
					}
					cnt0=0;
					cntempty=0;
					if(board[i][j-1]==-1)
					{
						for(k=j-1;board[i][k]==-1;--k)
						{
							++cntempty;
							if(k==-1)
								break;
						}
					}
				}
			}
		}
		if(cnt0)
		{
			if(cntempty+cnt0>=K)
					{
						if(cnt0>=K)
						{
							conscount1=conscount0=0;
							for(l=0;l<i;++l)
							{
								if(board[l][j]==0)
								{
									++conscount0;
								}
								else
								{
									conscount0=0;
								}
								if(conscount0>=K)
								{
									evalvalue+=1000000;
									break;
								}
							}
						}
						evalvalue+=pow(K,cnt0);
					}
		}
		else if(cnt1)
		{
			if(cntempty+cnt1>=K)
					{
						if(cnt1>=K)
						{
							conscount1=conscount0=0;
							for(l=0;l<i;++l)
							{
								if(board[l][j]==1)
								{
									++conscount1;
								}
								else
								{
									conscount1=0;
								}
								if(conscount1>=K)
								{
									evalvalue-=1000000;
									break;
								}
							}
						}
						evalvalue-=pow(K,cnt1);
					}
		}
	}
	//cout<<evalvalue<<endl;
	// Diags-downwards
	for(i=0;i<=M-K;++i)
	{
		for(j=0;j<=N-K;++j)
		{
			cnt0=cnt1=cntempty=0;
			for(k=0;k<K;++k)
			{
				if(board[i+k][j+k]==-1)
				{
					++cntempty;
				}
				else if(board[i+k][j+k]==0)
				{
					++cnt0;
					if(cnt1)
					{
						if(cntempty+cnt1>=K)
						{
							if(cnt1>=K)
							{
								conscount1=conscount0=0;
								for(l=0;l<k;++l)
								{
									if(board[i+l][j+l]==1)
									{
										++conscount1;
									}
									else
									{
										conscount1=0;
									}
									if(conscount1>=K)
									{
										evalvalue-=1000000;
										break;
									}
								}
							}
							evalvalue-=pow(K,cnt1);
						}
						cnt1=0;
						cntempty=0;
						if(board[i+k-1][j+k-1]==-1)
						{
							for(kdash=k-1;board[i+kdash][j+kdash]!=-1;--kdash)
							{
								++cntempty;
								if(kdash==-1)
									break;
							}
						}
					}
				}
				else if(board[i+k][j+k]==1)
				{
					++cnt1;
					if(cnt0)
					{
						if(cntempty+cnt0>=K)
						{
							if(cnt0>=K)
							{
								conscount1=conscount0=0;
								for(l=0;l<k;++l)
								{
									if(board[i+l][j+l]==0)
									{
										++conscount0;
									}
									else
									{
										conscount0=0;
									}
									if(conscount0>=K)
									{
										evalvalue+=1000000;
										break;
									}
								}
							}
							evalvalue+=pow(K,cnt0);
						}
						cnt0=0;
						cntempty=0;
						if(board[i+k-1][j+k-1]==-1)
						{
							for(kdash=k-1;board[i+kdash][j+kdash]==-1;--kdash)
							{
								++cntempty;
								if(kdash==-1)
									break;
							}
						}
					}
				}
			}
			if(cnt0)
			{
				if(cntempty+cnt0>=K)
						{
							if(cnt0>=K)
							{
								conscount1=conscount0=0;
								for(l=0;l<k;++l)
								{
									if(board[i+l][j+l]==0)
									{
										++conscount0;
									}
									else
									{
										conscount0=0;
									}
									if(conscount0>=K)
									{
										evalvalue+=1000000;
										break;
									}
								}
							}
							evalvalue+=pow(K,cnt0);
						}
			}
			else if(cnt1)
			{
				if(cntempty+cnt1>=K)
						{
							if(cnt1>=K)
							{
								conscount1=conscount0=0;
								for(l=0;l<k;++l)
								{
									if(board[i+l][j+l]==1)
									{
										++conscount1;
									}
									else
									{
										conscount1=0;
									}
									if(conscount1>=K)
									{
										evalvalue-=1000000;
										break;
									}
								}
							}
							evalvalue-=pow(K,cnt1);
						}
			}
		}
	}
	//cout<<evalvalue<<endl;
	// Diags-upwards
	for(i=M-1;i>=K-1;--i)
	{
		for(j=0;j<=N-K;++j)
		{
			cnt0=cnt1=cntempty=0;
			for(k=0;k<K;++k)
			{
				if(board[i-k][j+k]==-1)
				{
					++cntempty;
				}
				else if(board[i-k][j+k]==0)
				{
					++cnt0;
					if(cnt1)
					{
						if(cntempty+cnt1>=K)
						{
							if(cnt1>=K)
							{
								conscount1=conscount0=0;
								for(l=0;l<k;++l)
								{
									if(board[i-l][j+l]==1)
									{
										++conscount1;
									}
									else
									{
										conscount1=0;
									}
									if(conscount1>=K)
									{
										evalvalue-=1000000;
										break;
									}
								}
							}
							evalvalue-=pow(K,cnt1);
						}
						cnt1=0;
						cntempty=0;
						if(board[i-k+1][j+k-1]==-1)
						{
							for(kdash=k-1;board[i-kdash][j+kdash]!=-1;--kdash)
							{
								++cntempty;
								if(kdash==-1)
									break;
							}
						}
					}
				}
				else if(board[i-k][j+k]==1)
				{
					++cnt1;
					if(cnt0)
					{
						if(cntempty+cnt0>=K)
						{
							if(cnt0>=K)
							{
								conscount1=conscount0=0;
								for(l=0;l<k;++l)
								{
									if(board[i-l][j+l]==1)
									{
										++conscount0;
									}
									else
									{
										conscount0=0;
									}
									if(conscount0>=K)
									{
										evalvalue+=1000000;
										break;
									}
								}
							}
							evalvalue+=pow(K,cnt0);
						}
						cnt0=0;
						cntempty=0;
						if(board[i-k+1][j+k-1]==-1)
						{
							for(kdash=k-1;board[i-kdash][j+kdash]==-1;--kdash)
							{
								++cntempty;
								if(kdash==-1)
									break;
							}
						}
					}
				}
			}
			if(cnt0)
			{
				if(cntempty+cnt0>=K)
						{
							if(cnt0>=K)
							{
								conscount1=conscount0=0;
								for(l=0;l<k;++l)
								{
									if(board[i-l][j+l]==0)
									{
										++conscount0;
									}
									else
									{
										conscount0=0;
									}
									if(conscount0>=K)
									{
										evalvalue+=1000000;
										break;
									}
								}
							}
							evalvalue+=pow(K,cnt0);
						}
			}
			else if(cnt1)
			{
				if(cntempty+cnt1>=K)
						{
							if(cnt1>=K)
							{
								conscount1=conscount0=0;
								for(l=0;l<k;++l)
								{
									if(board[i-l][j+l]==1)
									{
										++conscount1;
									}
									else
									{
										conscount1=0;
									}
									if(conscount1>=K)
									{
										evalvalue-=1000000;
										break;
									}
								}
							}
							evalvalue-=pow(K,cnt1);
						}
			}
		}
	}
	// cout<<"Evaluation for \n";
	// printBoard();
	//cout<<evalvalue<<endl;
	return evalvalue;
}
int tictactoe::value(int depth,int alpha,int beta)
{
	++cnt;
	if(depth==D)
	{
		return evaluate()/depth;
	}
	else if(nextTurn()==0)
	{
		return max_value(depth,alpha,beta);
	}
	else if(nextTurn()==1)
	{
		return min_value(depth,alpha,beta);
	}
	else if(nextTurn()==-1)
	{
		// printBoard();
		// cout<<"Heuristic Value = "<<evaluate()/depth<<endl;
		return evaluate()/depth;
	}
}
int tictactoe::max_value(int depth,int alpha,int beta)
{
	int minmax=-500000000,i,j,m,posi=-1,posj=-1,flag=0;

		for(j=0;j<N;++j)
		{
		    for(i=M-1;i>-1;--i)
            {
                if(board[i][j]==-1)
                {
                	break;
                }
            }
            if(i==-1)
            {
            	continue;
            }
			if(board[i][j]==-1)
			{
				board[i][j]=0;
				m=value(depth+1,alpha,beta);
				//cout<<"max m="<<m<<endl;
				if(m>minmax)
				{
					minmax=m;
					posi=i;
					posj=j;
				}
				if(minmax>=beta)
				{
					flag=1;
					board[i][j]=-1;
					break;
				}
				else if(minmax>alpha)
				{
					alpha=minmax;
				}
				board[i][j]=-1;
			}
			if(flag==1)
				break;
		}
	// if(depth==0)
	// {
	// 	cout<<"max printing... no. of nodes explored..."<<cnt<<endl;
	// 	board[posi][posj]=0;
	// 	printBoard();
	// }
	if(depth==0)
	{
		posx=posi;
		posy=posj;
	}
	return minmax;
}
int tictactoe::min_value(int depth,int alpha,int beta)
{
	int minmax=500000000,i,j,m,posi=-1,posj=-1,flag=0;

		for(j=0;j<N;++j)
		{
		    for(i=M-1;i>-1;--i)
            {
                if(board[i][j]==-1)
                {
                	break;
                }
            }
            if(i==-1)
            {
            	continue;
            }
			if(board[i][j]==-1)
			{
				board[i][j]=1;
				m=value(depth+1,alpha,beta);
				//cout<<"min m="<<m<<endl;
				if(m<minmax)
				{
					minmax=m;
					posi=i;
					posj=j;
				}
				if(minmax<=alpha)
				{
					flag=1;
					board[i][j]=-1;
					break;
				}
				else if(minmax<beta)
				{
					beta=minmax;
				}
				board[i][j]=-1;
			}
			if(flag==1)
                break;
		}


	// if(depth==0)
	// {
	// 	cout<<"min printing... no. of nodes explored..."<<cnt<<endl;
	// 	board[posi][posj]=1;
	// 	printBoard();
	// }
	if(depth==0)
	{
		posx=posi;
		posy=posj;
	}
	return minmax;
}
bool tictactoe::checkGameOver()
{
	int i,j;
	if(checkWinPos()!=-1)
		return true;
	for(i=0;i<M;++i)
	{
		for(j=0;j<N;++j)
		{
			if(board[i][j]==-1)
			{
				isGameOver=false;
				return false;
			}
		}
	}
	isGameOver=true;
	return true;
}

