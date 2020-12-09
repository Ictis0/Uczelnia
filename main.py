# Sebastian Ratańczuk 44476
import numpy as np, math, random
from sklearn import datasets
from sklearn.model_selection import train_test_split


class MLP:
    def __init__(self, hidden=100, epochs=100, eta=0.1, shuffle=True):
        self.hidden = hidden
        self.epochs = epochs
        self.eta = eta
        self.shuffle = shuffle
        self._sigmoid = lambda x: 1 / (1 + math.exp(1) ** x)

    def _forward(self, x):
        out_hidden = x.dot(self.w_h) + self.b_h
        out_hidden_sigmoid = self._sigmoid(out_hidden)
        out_activated = out_hidden_sigmoid.dot(self.w_out) + self.b_out
        out_activated_sigmoid = self._sigmoid(out_activated)
        return out_hidden_sigmoid, out_activated_sigmoid

    def _compute_cost(self, y, output):
        return sum((np.sum(-(y * np.log2(output)) + (1 - y) * np.log2(1 - output), axis=1)))

    def fit(self, x_train, y_train):
        self.w_h = np.random.normal(0, 0.1, (x_train.shape[1], self.hidden))
        self.b_h = np.zeros(self.hidden)
        self.w_out = np.random.normal(0, 0.1, (self.hidden, y_train.shape[1]))
        self.b_out = np.zeros(y_train.shape[1])

        if self.shuffle:
            tmp = np.hstack((x_train, y_train))
            np.random.shuffle(tmp)
            x_train = tmp[:, :x_train.shape[1]]
            y_train = tmp[:, x_train.shape[1]:]

        _, out = self._forward(x_train)
        print(self._compute_cost(y_train, out))

    def predict(self, x):
        pass


irys = datasets.load_iris()
irys_data = irys.data
irys_labels = irys.target
irys_labels_coded = np.zeros((irys_labels.size, 3))

for i in range(irys_labels.size):
    irys_labels_coded[i, irys_labels[i]] = 1

x_train, x_test, y_train, y_test = train_test_split(irys_data, irys_labels_coded, random_state=13)

mlp = MLP()
mlp.fit(x_train, y_train)
