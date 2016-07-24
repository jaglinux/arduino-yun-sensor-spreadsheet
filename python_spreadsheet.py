from __future__ import print_function
import httplib2
import os
import json
import datetime

from apiclient import discovery
import oauth2client
from oauth2client import client
from oauth2client import tools

try:
    import argparse
    flags = argparse.ArgumentParser(parents=[tools.argparser]).parse_args()
except ImportError:
    flags = None

# If modifying these scopes, delete your previously saved credentials
# at ~/.credentials/sheets.googleapis.com-python-quickstart.json
SCOPES = 'https://www.googleapis.com/auth/drive'
#SCOPES = 'https://www.googleapis.com/feeds'
CLIENT_SECRET_FILE = 'client_secret.json'
APPLICATION_NAME = 'Google Sheets API test rer'


def get_credentials():
    """Gets valid user credentials from storage.

    If nothing has been stored, or if the stored credentials are invalid,
    the OAuth2 flow is completed to obtain the new credentials.

    Returns:
        Credentials, the obtained credential.
    """
    home_dir = os.path.expanduser('~')
    credential_dir = os.path.join(home_dir, '.credentials')
    if not os.path.exists(credential_dir):
        os.makedirs(credential_dir)
    credential_path = os.path.join(credential_dir,
                                   'sheets.googleapis.com-python-quickstart.json')

    store = oauth2client.file.Storage(credential_path)
    credentials = store.get()
    if not credentials or credentials.invalid:
        flow = client.flow_from_clientsecrets(CLIENT_SECRET_FILE, SCOPES)
        flow.user_agent = APPLICATION_NAME
        if flags:
            credentials = tools.run_flow(flow, store, flags)
        else: # Needed only for compatibility with Python 2.6
            credentials = tools.run(flow, store)
        print('Storing credentials to ' + credential_path)
    return credentials

def main():
    
    credentials = get_credentials()
    http = credentials.authorize(httplib2.Http())
    discoveryUrl = ('https://sheets.googleapis.com/$discovery/rest?'
                    'version=v4')
    service = discovery.build('sheets', 'v4', http=http,
                              discoveryServiceUrl=discoveryUrl)
    #Once you create a spreadsheet, if we open the spreadsheet, in the URL we will see the spreadsheet ID.
    spreadsheetId = '1Pi1wAqHQivrJQGekIIEqzOol-Z6bp4CGhvaYmIpXKPA'
    now = datetime.datetime.now()
    current_time = now.strftime("%Y-%m-%d-%H:%M")
    print (current_time)
    #myBody = {u'range': u'Sheet1!A2:B3', u'values': [[u'date', u'512'], [u'date2', u'600']], u'majorDimension': u'ROWS'}
    myBody = {u'range': u'Sheet1!A1:B1', u'values': [[current_time, u'444']], u'majorDimension': u'ROWS'}
    rangeUpdate = 'Sheet1!A1:B1'
    update = service.spreadsheets().values().update(spreadsheetId=spreadsheetId, range=rangeUpdate, valueInputOption='RAW', body=myBody).execute()
   
    rangeName = 'Sheet1!A1:B1'
    result = service.spreadsheets().values().get(spreadsheetId=spreadsheetId, range=rangeName).execute()
    values = result.get('values', [])



    if not values:
        print('No data found.')
    else:
        print('Name, Major:')
        for row in values:
            # Print columns A and E, which correspond to indices 0 and 4.
            print('%s, %s' % (row[0], row[1]))


if __name__ == '__main__':
    main()
