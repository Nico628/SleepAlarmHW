/*
 * scriptformusic.h
 *
 *  Created on: Feb 27, 2019
 *      Author: nico
 */

void Init_Music(void);
int putcharMusic(int c);
int getcharMusic( void );
int MusicTestForReceivedData(void);
void MusicFlush(void);
void BTOutMessage(char *message);
void SignalToPlay(void);
