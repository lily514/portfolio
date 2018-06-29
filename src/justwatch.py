from justwatch import JustWatch

just_watch = JustWatch(country='US')

provider_names = {
	10: 'AmazonPrime',
	8: 'Netflix',
	2: 'iTunes',
	3: 'GooglePlay',
	15: 'Hulu',
}

provider_ids = {
	'AmazonPrime': 10,
	'Netflix': 8,
	'iTunes': 2,
	'GooglePlay': 3,
	'Hulu': 15,
}

default_providers = set(provider_names.keys())
user_providers = set([8, 10, 15])


def search(my_query):
	results = just_watch.search_for_item(query=my_query)
	#TODO: check that title matches
	top_result = results['items'][0]
	title = top_result['title']
	object_type = top_result['object_type']
	providers = {}
	for offers in top_result['offers']:
		id = offers['provider_id']
		values = {}
		values['monetization_type'] = offers['monetization_type']
		values['element_count'] = offers['element_count']
		if values['monetization_type'] == 'flatrate':
			values['retail_price'] = offers['retail_price']
		providers[id] = values
	print(providers)



