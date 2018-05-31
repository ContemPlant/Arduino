

def health( values):
	health = 0
	for i in range(0,100):
		if i == 99:
			health += (1/(2**i)) * values[i]
		else:
			health += (1/(2**(i+1))) * values[i]

	return health

def growth( health):
	if health >= 0.75:
		return 0.1 * health # 0.1 depends on in which time sequence the values are send

	return 0

	