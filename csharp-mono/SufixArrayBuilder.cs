using System;
using System.Diagnostics;
using System.Text;
using System.Collections.Generic;
using System.IO;

namespace bioinf_sufix_array
{
	class MainClass
	{
		// Constants
		public const bool LTYPE = false;
		public const bool STYPE = true;
		public const bool BUCKET_END = true;
		public const bool BUCKET_START = false;

		// Algorithm parameters
		public static bool DEBUG = false;
		public static bool TEST = false;
		public static int d = 2;
		public static string test = "mmiissiissiippii";

		public static void Main (string[] args)
		{
			if (args.Length != 2 && args.Length != 3) {
				Console.Error.WriteLine ("Provide args: d out | d out in");
				return;
			}

			d = int.Parse (args [0]);
            string input = args[1];
            string output = args[2];

			if (File.Exists (output)) {
				File.Delete (output);
			}

			List<string> lines = null;

			if (args.Length == 3) {
				lines = new List<string> (File.ReadLines (input));
			}

            List<string> newLines = new List<string>();


            foreach (String line in lines) {
				if (line.StartsWith (">")) {
					continue;
				}
				newLines.Add (line);
			}

			byte[] b = (TEST) ? Encoding.ASCII.GetBytes (test + "\0") :
				Encoding.ASCII.GetBytes (String.Join ("", newLines) + "\0");
			int[] s = new int[b.Length];

			int[] sa = new int[s.Length];

			for (int i = 0; i < b.Length; i++) {
				s [i] = (int)b [i];
			}
			lines = null;
			newLines = null;
			b = null;
			// start measurment of time and memory
			System.GC.Collect ();
			Process currentProcess =
				System.Diagnostics.Process.GetCurrentProcess();
			long memoryBefore = currentProcess.PeakWorkingSet64;
			Stopwatch sw = new Stopwatch();
			sw.Start ();
			SA_DS (s, sa, 256, 0);
			sw.Stop ();
			long memoryAfter = currentProcess.PeakWorkingSet64;

//			Console.Out.WriteLine ("Is sorted:" + isSorted (sa, s, s.Length));
//			Console.Out.WriteLine (sw.ElapsedMilliseconds + " "
//			                       + (memoryAfter - memoryBefore));
			using (StreamWriter outputFile = new StreamWriter (output, true)) {
				for (int i = 0; i < sa.Length; i++) {
					outputFile.WriteLine (sa [i]);
				}
			}
		}

		public static void SA_DS (int[] s, int[] SA, int alphabetSize, int
		                          recursionLevel)
		{
			bool[] t = new bool[s.Length];
			mapLSType (s, t);
			int[] P1 = new int[s.Length];
			int n1 = getDCriticalPointers (s, t, P1);

			int[] b = new int[n1];
			int[] a = new int[n1];

			int[] sw = new int[s.Length];
			for (int i = 0; i < sw.Length; i++) {
				if (t [i]) {
					sw [i] = (2 * s [i] + 1);
				} else {
					sw [i] = (2 * s [i]);
				}
			}

			int[] P_1 = new int[n1];
			for (int i = 0; i < n1; i++) {
				P_1 [i] = P1 [i];
			}

			if (DEBUG)
				logDebugLine ("Sw: " + String.Join (" ", sw));

			if (DEBUG)
				logDebugLine ("P1: " + String.Join (" ", P1));
			bucketSortLS (P1, a, s, t, n1, d + 1);
			for (int i = 0; i < d + 2; i++) {
				if (i % 2 == 0) {
					bucketSort (a, b, s, n1, alphabetSize, d + 1 - i);
					P1 = b;
				} else {
					bucketSort (b, a, s, n1, alphabetSize, d + 1 - i);
					P1 = a;
				}
				if (DEBUG)
					logDebugLine ("Sort " + (i + 1) + ":"
					              + String.Join (" ", P1));
			}
			if (DEBUG)
				logDebugLine ("Complete pass: " + String.Join (" ", P1));
			for (int i = 0; i < t.Length; i++) {
				if (t [i] == LTYPE) {
					logDebug ("0 ");
				} else {
					logDebug ("1 ");
				}
			}
			logDebugLine ("");
			int name = 0;

			int[] S1 = new int[n1];
			bool diff = false;
			for (int i = 0; i < n1; i++) {
				if (i == 0) {
					S1 [0] = name;
					continue;
				}
				diff = false;
				for (int j = 0; j < d + 2; j++) {
					if (sw [P1 [i - 1] + j] != sw [P1 [i] + j]) {
						diff = true;
						S1 [i] = ++name;
						break;
					}
				}
				if (!diff) {
					S1 [i] = name;
				}
			}

			int[] tmp = new int[s.Length];
			for (int i = 0; i < tmp.Length; i++) {
				tmp [i] = -1;
			}
			for (int i = 0; i < n1; i++) {
				tmp [P1 [i]] = S1 [i];
			}
			for (int i = 0, k = 0; i < s.Length; i++) {
				if (tmp [i] != -1) {
					S1 [k++] = tmp [i];
				}
			}
			if (DEBUG)
				logDebugLine ("S1: " + String.Join (" ", S1));

			int[] SA1 = new int[S1.Length];


			if (name < n1 - 1) {
				SA_DS (S1, SA1, name + 1, recursionLevel + 1);
				System.GC.Collect ();
			} else {
				SA1 = new int[S1.Length];
				//induce SA1 from S1
				logDebugLine ("Inducing SA1 from S1");

				for (int i = 0; i < SA1.Length; i++) {
					SA1 [S1 [i]] = i;
				}
				if (DEBUG)
					logDebugLine ("SA1: " + String.Join (" ", SA1));
			}
			//induce SA from SA1
			if (DEBUG)
				logDebugLine ("Inducing SA from SA1: " +
				              String.Join (" ", SA1));

			for (int i = 0; i < SA.Length; i++) {
				SA [i] = -1;
			}
			int[] B = getBuckets (s, alphabetSize, BUCKET_END);

			for (int i = SA1.Length - 1; i >= 0; i--) {
				// check if is LMS and add to end of the bucket
				if (t [P_1 [SA1 [i]]] == STYPE &&
				    	t [P_1 [SA1 [i]] - 1] == LTYPE) {
					SA [--B [s [P_1 [SA1 [i]]]]] = P_1 [SA1 [i]];
				}
			}
			if (DEBUG)
				logDebugLine ("Step 1: " + String.Join (" ", SA));


			B = getBuckets (s, alphabetSize, BUCKET_START);
			for (int i = 0; i < SA.Length; i++) {
				if (SA [i] > 0) {
					if (t [SA [i] - 1] == LTYPE) {
						SA [B [s [SA [i] - 1]]++] = SA [i] - 1;
					}
				}
			}
			if (DEBUG)
				logDebugLine ("Step 2: " + String.Join (" ", SA));

			B = getBuckets (s, alphabetSize, BUCKET_END);
			for (int i = SA.Length - 1; i >= 0; i--) {
				if (SA [i] > 0) {
					if (t [SA [i] - 1] == STYPE) {
						SA [--B [s [SA [i] - 1]]] = SA [i] - 1;
					}
				}
			}
			if (DEBUG)
				logDebugLine ("Step 3: " + String.Join (" ", SA));
		}

		public static int[] getBuckets (int[] s, int alphabetSize, bool getEnds)
		{
			int[] bucket = new int[alphabetSize];
			for (int i = 0; i < s.Length; i++) {
				bucket [s [i]]++;
			}
			int sum = 0;
			for (int i = 0; i < alphabetSize; i++) {
				sum += bucket [i];
				bucket [i] = getEnds ? sum : sum - bucket [i];
			}
			return bucket;
		}

		public static int getDCriticalPointers (int[] s, bool[] t, int[] P1)
		{
			int k = 0;
			for (int i = 1; i < s.Length; i++) {
				if (t [i] == STYPE && t [i - 1] == LTYPE) {
					P1 [k++] = i;
				} else if (!(t [i + 1] == STYPE && t [i] == LTYPE)) {
					if (k > 0 && P1 [k - 1] == i - d) {
						P1 [k++] = i;
					}
				}
			}
			return k;
		}

		public static void mapLSType (int[] s, bool[] t)
		{
			for (int i = s.Length - 1; i >= 0; i--) {
				if (i == s.Length - 1) {
					t [i] = STYPE;
				} else if (s [i] > s [i + 1] ||
				           s [i] == s [i + 1] && t [i + 1] == LTYPE) {
					t [i] = LTYPE;
				} else {
					t [i] = STYPE;
				}
			}
		}

		public static void bucketSort (int[] a, int[] b, int[] s, int n1,
		                               int alphabetSize, int d)
		{

			int[] c = new int[alphabetSize];

			for (int i = 0, j = 0; i < n1; i++) {
				j = a [i] + d;
				j = (j > s.Length - 1) ? s.Length - 1 : j;
				c [s [j]]++;
			}

			for (int i = 0, sum = 0; i < alphabetSize; i++) {
				int t = c [i];
				c [i] = sum;
				sum += t;
			}

			for (int i = 0, j = 0; i < n1; i++) {
				j = a [i] + d;
				j = (j > s.Length - 1) ? s.Length - 1 : j;
				b [c [s [j]]++] = a [i];
			}
		}

		public static void bucketSortLS (int[] a, int[] b, int[] s, bool[] t,
		                                 int n1, int d)
		{

			int[] c = {0, n1 - 1};

			for (int i = 0, j = 0;  i < n1; i++) {
				j = a[i] + d;
				j = (j > s.Length - 1) ? s.Length - 1 : j;
				if (t[j] == STYPE) {
					b[c[1]--] = a[i];
				} else {
					b[c[0]++] = a[i];
				}
			}
		}

		public static void logDebugLine(string s) {
			if (DEBUG) {
				Console.Out.WriteLine (s);
			}
		}

		public static void logDebug(string s) {
			if (DEBUG) {
				Console.Out.Write (s);
			}
		}

		static int sless(int[] s, int p1, int p2, int n) {
			for(int i=0; i<n; i++) {
				if (s[p1 + i] < s[p2 + i]) return 1;
				if (s[p1 + i] > s[p2 + i]) return -1;
			}
			return 0;
		}

		// test if SA is sorted for the input string s
		static bool isSorted(int[] SA, int[] s, int n) {
			for(int i = 0;  i < n-1;  i++) {
				int d=SA[i]<SA[i+1]?n-SA[i+1]:n-SA[i];
				int rel=sless(s, SA[i], SA[i+1], d);
				if(rel==-1 || rel==0 && SA[i+1]>SA[i])
					return false;
			}
			return true;
		}

	}
}
