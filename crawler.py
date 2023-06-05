import requests

API_URL = 'https://api.reddit.com'

SUBREDDIT_NAME = 'philosophy'
POST_LIMIT = 500
COMENTS_PER_POST_LIMITS = 100

OUTPUT_FILE = 'training_data.txt'

url = f'{API_URL}/r/{SUBREDDIT_NAME}/new.json?limit={POST_LIMIT}'
headers = {'User-Agent': 'YOUR_USER_AGENT'}
response = requests.get(url, headers=headers)

data = response.json()

for post in data['data']['children']:
    post_title = post['data']['title']
    post_content = post['data']['selftext']
    post_id = post['data']['id']

    with open(OUTPUT_FILE, 'a') as file:
 
        file.write(f'{post_content}')

    comments_url = f'{API_URL}/r/{SUBREDDIT_NAME}/comments/{post_id}.json?limit={COMENTS_PER_POST_LIMITS}'  # Fetch 5 comments per post
    comments_response = requests.get(comments_url, headers=headers)
    comments_data = comments_response.json()

    # Check if comments exist for the post
    if 'data' in comments_data and 'children' in comments_data['data']:
        # Iterate over the comments
        for comment in comments_data['data']['children']:
            if 'data' in comment and 'body' in comment['data']:
                comment_body = comment['data']['body']

                # Append comment to the output file
                with open(OUTPUT_FILE, 'a') as file:
                    file.write(f'{comment_body}')

print('Post content with comments saved to', OUTPUT_FILE)
