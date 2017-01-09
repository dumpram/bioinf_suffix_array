import sys
import pdb

class SADS():

    def __init__(self, input_file, min_dist):
        '''
        Creates new instance of SA-DS object, capable of creating suffix array using SA-DS algorithm.
        '''

        assert min_dist >= 2
        self.min_dist = min_dist
        self.read_data(input_file)
        self.do_sads(self.input_data)

    def read_data(self, input_file):
        with open(input_file, 'r', encoding='utf-8') as f:
            self.input_data = []
            for line in f.readlines():
                if not line.startswith('>'):
                    self.input_data += [ord(x) for x in line]

        self.input_sample = self.input_data[0:60]
        self.input_sample.append(ord('$'))

        self.input_data.append(ord('$'))

    def do_sads(self, input_string):
        '''
        Does SA-DS algorithm over input string and returs suffix array
        '''

        self.input_string = input_string
        self.input_length = len(input_string)

        self.distinguish_L_and_S()
        print(self.t)
        self.find_critical_substrings()
        print(len(self.p_1))
        print(self.p_1)
        self.bucket_sort()
        print(len(self.s_1))
        print(self.s_1)

    def distinguish_L_and_S(self):
        '''
        Goes over input string and sorts out characters into L and S group

        If S[i] < S[i+1] or S[i] = S[i+1] charcater is classified as S type.
        If S[i] > S[i+1] or S[i] = S[i+1] charcater is classified as L type.
        '''

        self.t = [True]
        current_mark = True
        for index in reversed(range(len(self.input_string)-1)):
            if self.input_string[index] != self.input_string[index+1]:
                current_mark = (self.input_string[index] < self.input_string[index+1])
            self.t.insert(0,current_mark)

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
        It's worth mentioning that result array is sorted.
        '''

        # find LMS characters first
        lms_chars = []
        s_found = False
        for index in range(len(self.t)):
            if self.t[index] and not s_found:
                lms_chars.append(index)
                s_found = True
                continue
            if not self.t[index] and s_found:
                s_found = False

        self.p_1 = [lms_chars[0]]   # append only first for now, rest of them will be appended later

        # find the rest of the critical characters
        for lms_index in range(len(lms_chars)-1):
            current_lms = lms_chars[lms_index]
            next_lms = lms_chars[lms_index+1]

            while (next_lms - current_lms - 1) > self.min_dist:     # append all d-crit chars between two lms chars
                current_lms += self.min_dist
                self.p_1.append(current_lms)

            self.p_1.append(next_lms)   # append lms as well
        
    def bucket_sort(self):
        '''
        Bucket sorts all the d-critical substrings according to their omega weight and generates S1 array
        '''

        omega = lambda i: 2 * self.input_string[i] + self.t[i]

        p_1_ordered = list(zip(self.p_1, range(len(self.p_1))))  # we need indicies of elements inside P1 for later

        for current_d in reversed(range(self.min_dist+2)):       # for each index in substring, starting from LSD
            map_buckets = {}                                     # init buckets
    
            for crit_entry in p_1_ordered:                       # crit_entry = (critical index, index of crit index inside P1)
                crit_index = crit_entry[0]
                current_index = (crit_index+current_d) if ((crit_index+current_d) < self.input_length) else (self.input_length-1)
                current_omega = omega(current_index)
                if not current_omega in map_buckets:
                    map_buckets[current_omega] = []
                map_buckets[current_omega].append(crit_entry)
            
            p_1_ordered = []                                     # they are now in map_buckets, free up memory

            key_entries_sorted = sorted(map_buckets.items())     # sort over keys
            map_buckets = None                                   # they are now in key_entries_sorted, free up memory


            if current_d != 0:
                for key_entry in key_entries_sorted:             # reorder, key_entry = (bucket map key, list of (crit_entry))
                    p_1_ordered.extend(key_entry[1])

                key_entries_sorted = None                        # free up memory

            else:                                                # if this is the last pass create S1
                s_index = 0
                prev_substring = None
                self.s_1 = [None] * len(self.p_1)                # generate S1 array from sorted crit indicies
                duplicate_index = False
                for key_entry in key_entries_sorted:
                    for crit_entry in key_entry[1]:
                        current_substring = self.get_d_critical_substring(crit_entry[0])

                        if (prev_substring != None) and (prev_substring != current_substring):
                            s_index += 1
                        else:
                            duplicate_index = True

                        self.s_1[crit_entry[1]] = s_index
                        prev_substring = current_substring
                        
                if duplicate_index:
                    # recursive call
                    # new_sads = SADS()
                    # new_sads.do_sads(self.s_1)
                    pass

    def get_d_critical_substring(self, index):
        '''
        Returnes d-critical substring for given d-critical index

        Since d-critical substring's is d+2 characters long, possible
        lack of characters is solved by appending string's last character
        (multiple times if necessary)
        '''

        if index + self.min_dist + 2 <= self.input_length:
            end_index = index + self.min_dist + 2
            return self.input_string[index:end_index]
        else:
            indices_missing = (index + self.min_dist + 2 - self.input_length)
            return self.input_string[index:self.input_length] + (indices_missing * [self.input_string[-1]])


def main():
    sads = SADS(sys.argv[1], int(sys.argv[2]))

if __name__ == '__main__':
    main()