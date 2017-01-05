import sys


class SADS():

    def __init__(self, input_file):
        self.read_data(input_file)
        self.do_sads(self.input_sample)

    def read_data(self, input_file):
        with open(input_file, 'r', encoding='utf-8') as f:
            self.input_data = []
            for line in f.readlines():
                if not line.startswith('>'):
                    self.input_data.append(line)
        self.input_sample = self.input_data[0][:-1]
        self.input_sample += '$'
        self.input_data = "".join(self.input_data)

    def do_sads(self, input_string):
        self.distinguish_L_and_S(input_string)
        self.find_critical_characters()

    def distinguish_L_and_S(self, input_string):
        self.t = []
        current_mark = False
        for index in range(len(input_string)-1):
            if input_string[index] != input_string[index+1]:
                current_mark = (input_string[index] < input_string[index+1])
            self.t.append(current_mark)
        self.t.append(True)

    def find_critical_characters(self):
        #find LMS characters first
        self.p_1 = []
        s_found = False
        for index in range(len(self.t)):
            if self.t[index] and not s_found:
                self.p_1.append(index)
                s_found = True
                continue
            if not self.t[index] and s_found:
                s_found = False

def main():
    sads = SADS(sys.argv[1])
    print(sads.t)
    print(len(sads.t))
    print(sads.p_1)

if __name__ == '__main__':
    main()