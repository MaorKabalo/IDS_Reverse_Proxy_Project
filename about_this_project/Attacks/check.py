import requests

def check_malicious_ip(api_key, ip_address):
    url = f'https://api.abuseipdb.com/api/v2/check?ipAddress={ip_address}'
    headers = {
        'Key': api_key,
    }

    try:
        response = requests.get(url, headers=headers)
        data = response.json()

        # Check if the API response indicates that the IP is malicious
        if 'abuseConfidenceScore' in data and data['abuseConfidenceScore'] >= 80:
            print(f'The IP address {ip_address} is likely malicious (Confidence Score: {data["abuseConfidenceScore"]})')
        else:
            print(f'The IP address {ip_address} is not listed as malicious (Confidence Score: {data["abuseConfidenceScore"]})')
    except requests.exceptions.RequestException as e:
        print(f'Error: {e}')

if __name__ == "__main__":
    api_key = '8666736c54a1c5a1b1c434f9e4dcb5ef208eebe1089f1f1a7592ed28235ed915085c021131c200ac'
    ip_to_check = input("Enter an IP address to check: ")
    check_malicious_ip(api_key, ip_to_check)
