import random

def health( values):
	values[-1]
	health = 0.0
	for i in range(0,len(values)):
		if i == 99:
			health += (1/(2**i)) * values[i]
		else:
			health += (1/(2**(i+1))) * values[i]

	return health

def growth(health, factor, threshold):
	if health >= threshold:
		return factor * (health - threshold) # factor depends on in which time sequence the values are send

	return 0


def randomarray():
	array = []
	for i in range(0,99):
		array.append(random.uniform(0.5,1.0))

	return array

array = randomarray()
print(array)
print(health(array))
print(growth(health(array), 0.1, 0.6))