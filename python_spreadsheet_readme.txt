Follow below steps to create client_secret.json:

Use "https://console.developers.google.com/start/api?id=sheets.googleapis.com" to create or select a project in the Google Developers Console and automatically turn on the API. Click Continue, then Go to credentials.
At the top of the page, select the OAuth consent screen tab. Select an Email address, enter a Product name if not already set, and click the Save button.
Select the Credentials tab, click the Create credentials button and select OAuth client ID.
Select the application type Other, enter the name "Google Sheets API Quickstart", and click the Create button.
Click OK to dismiss the resulting dialog.
Click the file_download (Download JSON) button to the right of the client ID.
Move this file to your working directory and rename it client_secret.json.



Install the below module:
pip install --upgrade google-api-python-client

then run the script python_spreadsheet.py


