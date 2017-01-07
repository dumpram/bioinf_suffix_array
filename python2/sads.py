import sys

class SADS():

    def __init__(self, input_file, min_dist):
        '''
        Creates new instance of SA-DS object, capable of creating suffix array using SA-DS algorithm.
        '''

        assert min_dist >= 2
        self.min_dist = min_dist
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
        self.input_data += '$'

    def do_sads(self, input_string):
        '''
        Does SA-DS algorithm over input string and returs suffix array
        '''

        self.distinguish_L_and_S(input_string)
        self.find_critical_substrings()
        self.bucket_sort()

    def distinguish_L_and_S(self, input_string):
        '''
        Goes over input string and sorts out characters into L and S group

        If S[i] < S[i+1] or S[i] = S[i+1] charcater is classified as S type.
        If S[i] > S[i+1] or S[i] = S[i+1] charcater is classified as L type.
        '''


        self.t = []
        current_mark = False
        for index in range(len(input_string)-1):
            if input_string[index] != input_string[index+1]:
                current_mark = (input_string[index] < input_string[index+1])
            self.t.append(current_mark)
        self.t.append(True)

    def find_critical_substrings(self):
        '''
        Finds d-critical substrings determined by their d-critical characters.

        First locates LMS characters which are d-critical by definition.
        Then looks for the rest of d-critical characters, being at least
        self.min_dist positions away from last preceding LMS character and at least
        one position away from next succeeding LMS character.
        These characters determine d-critical substrings that start on d-critical
        character position and are d+2 characters long. In case of character beeing
        on the string's very end last character of string is taken as many times as
        needed to get substring to d+2 length. 
        '''

        # find LMS characters first
        self.p_1 = []
        s_found = False
        for index in range(len(self.t)):
            if self.t[index] and not s_found:
                self.p_1.append(index)
                s_found = True
                continue
            if not self.t[index] and s_found:
                s_found = False

        # find the rest of the critical characters
        rest_p_1 = []
        for lms_index in range(len(self.p_1)-1):
            current_lms = self.p_1[lms_index]
            next_lms = self.p_1[lms_index+1]

            while (next_lms - current_lms - 1) > self.min_dist:
                current_lms += self.min_dist
                rest_p_1.append(current_lms)
        
        self.p_1 += rest_p_1
        print(len(self.p_1))
        print(self.p_1)

    def bucket_sort(self):
        '''
        Bucket sorts all the d-critical substrings using P1 and B arrays
        '''
        pass


def main():
    sads = SADS(sys.argv[1], int(sys.argv[2]))
#    print(sads.t)
#    print(len(sads.t))

if __name__ == '__main__':
    main()