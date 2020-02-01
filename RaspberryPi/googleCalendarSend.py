from __future__ import print_function
import datetime
import pickle
import os.path
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
import requests;
import json;


# If modifying these scopes, delete the file token.pickle.
SCOPES = ['https://www.googleapis.com/auth/calendar.readonly']

def main():
    """Shows basic usage of the Google Calendar API.
    Prints the start and name of the next 10 events on the user's calendar.
    """
    creds = None
    # The file token.pickle stores the user's access and refresh tokens, and is
    # created automatically when the authorization flow completes for the first
    # time.
    if os.path.exists('token.pickle'):
        with open('token.pickle', 'rb') as token:
            creds = pickle.load(token)
    # If there are no (valid) credentials available, let the user log in.
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                'credentials.json', SCOPES)
            creds = flow.run_local_server()
        # Save the credentials for the next run
        with open('token.pickle', 'wb') as token:
            pickle.dump(creds, token)

    service = build('calendar', 'v3', credentials=creds)

    # Call the Calendar API
    
    '''' This works don't mess with it
    
    now = datetime.datetime.utcnow().isoformat() + 'Z' # 'Z' indicates UTC time
    
    #Set time as todays date, with 8 hour as its 8 hours behind
    
    tomorrow = (datetime.date.today() + datetime.timedelta(days=1)).isoformat() + 'T08:00:00.000000Z' '''
    
    
    
    now = (datetime.date.today()).isoformat() + 'T08:00:00.000000Z' # 'Z' indicates UTC time
    
    #Set time as todays date, with 8 hour as its 8 hours behind
    
    tomorrow = (datetime.date.today() + datetime.timedelta(days=1)).isoformat() + 'T08:00:00.000000Z'
    
    
    ''' To check the next things time
    now = (datetime.date.today() + datetime.timedelta(days=1)).isoformat() + 'T08:00:00.000000Z' # 'Z' indicates UTC time
    
    #Set time as todays date, with 8 hour as its 8 hours behind
    
    tomorrow = (datetime.date.today() + datetime.timedelta(days=2)).isoformat() + 'T08:00:00.000000Z' '''
    
    print (now)
    print (tomorrow)
    
    ''' This was to get the upcoming 10 events
    print('Getting the upcoming 10 events')
    
    
    events_result = service.events().list(calendarId='primary', timeMin=now,
                                        maxResults=10, singleEvents=True,
                                        orderBy='startTime').execute()
    events = events_result.get('items', []) '''
    
    
    print('Getting todays events')
    
    events_result = service.events().list(calendarId='primary', timeMin=now,
                                        timeMax=tomorrow,
                                        maxResults=10, singleEvents=True,
                                        orderBy='startTime').execute()
    events = events_result.get('items', [])
    

    if not events:
        print('No upcoming events found.')
    for event in events:
        start = event['start'].get('dateTime', event['start'].get('date'))
        end = event['end'].get('dateTime', event['end'].get('date'))
        print(start, event['summary'], end)
        
        data = {"event_name": event['summary'],
            "start_time": start,
            "end_time": end}
        
        r = requests.post('http://django-env.qgbzkwbhei.us-west-1.elasticbeanstalk.com/events/', auth=('admin', 'cpen391'), 
                data = data)

        print(r.status_code)
        

if __name__ == '__main__':
    main()